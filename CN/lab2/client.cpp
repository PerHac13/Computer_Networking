// client.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <filesystem>
#include <stdexcept>

class TCPClient
{
private:
    static const int BUFFER_SIZE = 4096;
    int sockfd;
    std::string hostname;
    int port;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    void initializeSocket()
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            throw std::runtime_error("Error opening socket");
        }

        struct hostent *server = gethostbyname(hostname.c_str());
        if (server == NULL)
        {
            throw std::runtime_error("Error: no such host");
        }

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(port);
    }

public:
    TCPClient(const std::string &hostname, int port)
        : hostname(hostname), port(port)
    {
        initializeSocket();
    }

    ~TCPClient()
    {
        close(sockfd);
    }

    void connectToServer()
    {
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            throw std::runtime_error("Error connecting to server");
        }
        std::cout << "Connected to server" << std::endl;
    }

    void sendFile(const std::string &filepath)
    {
        // Open file
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Error opening file: " + filepath);
        }

        // Send filename
        std::string filename = std::filesystem::path(filepath).filename();
        if (write(sockfd, filename.c_str(), filename.length()) < 0)
        {
            throw std::runtime_error("Error sending filename");
        }

        // Small delay to ensure filename is received separately
        usleep(100000);

        // Send file content
        while (!file.eof())
        {
            file.read(buffer, BUFFER_SIZE);
            std::streamsize bytes_read = file.gcount();

            if (bytes_read > 0)
            {
                if (write(sockfd, buffer, bytes_read) < 0)
                {
                    throw std::runtime_error("Error sending file content");
                }
            }
        }

        file.close();
        std::cout << "File sent successfully" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <hostname> <port> <filepath>" << std::endl;
        return 1;
    }

    try
    {
        TCPClient client(argv[1], std::stoi(argv[2]));
        client.connectToServer();
        client.sendFile(argv[3]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
// server.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>

class TCPServer
{
private:
    static const int BUFFER_SIZE = 4096;
    int sockfd, port;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    void initializeSocket()
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            throw std::runtime_error("Error opening socket");
        }

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            throw std::runtime_error("Error on binding");
        }
    }

    void receiveFile(int clientSocket)
    {
        // Receive filename
        char filename[256];
        bzero(filename, 256);
        if (read(clientSocket, filename, 255) < 0)
        {
            throw std::runtime_error("Error receiving filename");
        }

        std::cout << "Receiving file: " << filename << std::endl;

        // Create and write to file
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Error opening file for writing");
        }

        ssize_t n;
        while ((n = read(clientSocket, buffer, BUFFER_SIZE)) > 0)
        {
            file.write(buffer, n);
        }

        if (n < 0)
        {
            throw std::runtime_error("Error receiving file content");
        }

        file.close();
        std::cout << "File received successfully: " << filename << std::endl;
    }

public:
    TCPServer(int port) : port(port)
    {
        initializeSocket();
    }

    ~TCPServer()
    {
        close(sockfd);
    }

    void start()
    {
        listen(sockfd, 5);
        std::cout << "Server listening on port " << port << std::endl;

        while (true)
        {
            struct sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);

            int clientSocket = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            if (clientSocket < 0)
            {
                std::cerr << "Error on accept" << std::endl;
                continue;
            }

            std::cout << "New connection accepted" << std::endl;

            try
            {
                receiveFile(clientSocket);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }

            close(clientSocket);
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    try
    {
        TCPServer server(std::stoi(argv[1]));
        server.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

class TCPClient
{
public:
    TCPClient() : sock(-1) {}

    ~TCPClient()
    {
        if (sock != -1)
            close(sock);
    }

    void run()
    {
        createSocket();
        connectToServer();
        communicate();
    }

private:
    int sock;
    struct sockaddr_in server_address;

    void createSocket()
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Socket successfully created.\n";
    }

    void connectToServer()
    {
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT);

        // Convert IP address from text to binary format
        if (inet_pton(AF_INET, "198.168.247.29", &server_address.sin_addr) <= 0)
        {
            perror("Invalid address or address not supported");
            exit(EXIT_FAILURE);
        }

        if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        {
            perror("Connection to the server failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Connected to the server.\n";
    }

    void communicate()
    {
        char buffer[1024] = {0};
        std::string message;

        while (true)
        {
            // Input message from the user
            std::cout << "Enter message to send (type 'exit' to quit): ";
            std::getline(std::cin, message);

            // Send message to the server
            send(sock, message.c_str(), message.length(), 0);

            // Termination condition
            if (message == "exit")
            {
                std::cout << "Closing connection to the server.\n";
                break;
            }

            // Receive response from the server
            memset(buffer, 0, sizeof(buffer)); // Clear buffer
            int bytesRead = read(sock, buffer, sizeof(buffer));
            if (bytesRead <= 0)
            {
                std::cout << "Server disconnected or error occurred.\n";
                break;
            }
            std::cout << "Server: " << buffer << std::endl;
        }
    }
};

int main()
{
    TCPClient client;
    client.run();
    return 0;
}

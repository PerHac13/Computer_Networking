#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

class TCPServer
{
public:
    TCPServer() : server_fd(-1), client_fd(-1) {}

    ~TCPServer()
    {
        if (client_fd != -1)
            close(client_fd);
        if (server_fd != -1)
            close(server_fd);
    }

    void run()
    {
        createSocket();
        bindSocket();
        listenForConnections();
        acceptConnection();
        communicate();
    }

private:
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen;

    void createSocket()
    {
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Socket successfully created" << std::endl;
    }

    void bindSocket()
    {
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Socket successfully binded" << std::endl;
    }

    void listenForConnections()
    {
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        std::cout << "Server listening on port " << PORT << std::endl;
    }

    void acceptConnection()
    {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::cout << "Connection accepted" << std::endl;
    }

    void communicate()
    {
        char buffer[1024] = {0};
        std::string message;

        while (true)
        {
            // Receive message from the client
            memset(buffer, 0, sizeof(buffer)); // Clear buffer
            int bytesRead = read(client_fd, buffer, sizeof(buffer));
            if (bytesRead <= 0)
            {
                std::cout << "Client disconnected or error occurred.\n";
                break;
            }
            std::cout << "Client: " << buffer << std::endl;

            // Input response message from server
            std::cout << "Enter message to send (type 'exit' to quit): ";
            std::getline(std::cin, message);

            // Send message to the client
            send(client_fd, message.c_str(), message.length(), 0);

            // Termination condition
            if (message == "exit")
            {
                std::cout << "Closing connection with client.\n";
                break;
            }
        }
    }
};

int main()
{
    TCPServer server;
    server.run();
    return 0;
}

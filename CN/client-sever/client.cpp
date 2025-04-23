// client.cpp
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>

#define BUFFER_SIZE 1024

std::atomic<bool> running(true);

void receive_messages(int socket_fd)
{
    char buffer[BUFFER_SIZE];

    while (running)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0)
        {
            std::cout << "Server disconnected" << std::endl;
            running = false;
            break;
        }

        std::string message(buffer, 0, bytes_received);
        std::cout << message << std::endl;
    }
}

int main(int argc, char *argv[])
{
    // Check command line arguments
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port> <username>" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    int port = std::stoi(argv[2]);
    std::string username = argv[3];

    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address or address not supported" << std::endl;
        return 1;
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    std::cout << "Connected to server at " << server_ip << ":" << port << std::endl;

    // Send username to server
    send(client_socket, username.c_str(), username.length(), 0);

    // Start thread to receive messages
    std::thread receive_thread(receive_messages, client_socket);

    // Main thread handles sending messages
    std::string message;
    while (running)
    {
        std::getline(std::cin, message);

        if (message == "/quit")
        {
            running = false;
            break;
        }

        if (send(client_socket, message.c_str(), message.length(), 0) < 0)
        {
            std::cerr << "Failed to send message" << std::endl;
            running = false;
            break;
        }
    }

    // Clean up
    receive_thread.join();
    close(client_socket);
    return 0;
}
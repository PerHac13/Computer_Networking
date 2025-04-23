// server.cpp
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Structure to hold client information
struct ClientInfo
{
    int socket;
    std::string name;
    sockaddr_in address;
};

std::vector<ClientInfo> clients;
std::mutex clients_mutex;

void handle_client(int client_socket, const std::string &client_name)
{
    char buffer[BUFFER_SIZE];

    // Broadcast that a new client has joined
    std::string join_msg = client_name + " has joined the chat.";

    clients_mutex.lock();
    for (auto &client : clients)
    {
        if (client.socket != client_socket)
        {
            send(client.socket, join_msg.c_str(), join_msg.length(), 0);
        }
    }
    clients_mutex.unlock();

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0)
        {
            // Client disconnected
            break;
        }

        std::string message = client_name + ": " + std::string(buffer, 0, bytes_received);
        std::cout << message << std::endl;

        // Broadcast message to all other clients
        clients_mutex.lock();
        for (auto &client : clients)
        {
            if (client.socket != client_socket)
            {
                send(client.socket, message.c_str(), message.length(), 0);
            }
        }
        clients_mutex.unlock();
    }

    // Client disconnected, remove from list
    clients_mutex.lock();
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->socket == client_socket)
        {
            std::string leave_msg = it->name + " has left the chat.";
            clients.erase(it);

            // Notify other clients
            for (auto &client : clients)
            {
                send(client.socket, leave_msg.c_str(), leave_msg.length(), 0);
            }
            break;
        }
    }
    clients_mutex.unlock();

    close(client_socket);
}

int main(int argc, char *argv[])
{
    // Check command line arguments
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    int server_socket;
    struct sockaddr_in server_addr;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "Error setting socket options" << std::endl;
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error binding socket to port " << port << std::endl;
        return 1;
    }

    // Listen for connections
    if (listen(server_socket, 5) < 0)
    {
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }

    std::cout << "Server started on port " << port << std::endl;
    std::vector<std::thread> client_threads;

    while (true)
    {
        ClientInfo new_client;
        socklen_t client_len = sizeof(new_client.address);

        // Accept new connection
        new_client.socket = accept(server_socket, (struct sockaddr *)&new_client.address, &client_len);

        if (new_client.socket < 0)
        {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // Receive client name
        char name_buffer[100];
        recv(new_client.socket, name_buffer, sizeof(name_buffer), 0);
        new_client.name = std::string(name_buffer);

        // Add client to list
        clients_mutex.lock();
        clients.push_back(new_client);
        clients_mutex.unlock();

        std::cout << "New client connected: " << new_client.name
                  << " (" << inet_ntoa(new_client.address.sin_addr) << ")" << std::endl;

        // Create thread to handle client
        client_threads.push_back(std::thread(handle_client, new_client.socket, new_client.name));
        client_threads.back().detach();
    }

    // Close server socket
    close(server_socket);
    return 0;
}
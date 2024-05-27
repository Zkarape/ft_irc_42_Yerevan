#include "Server.hpp"

Server::Server() {}

Server::~Server()
{
    close(_ServerSocket);
    for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        close(*it);
    }

}

Server::Server(const Server &copy)
{
    *this = copy;
}

Server &Server::operator=(const Server &assign)
{
    *this = assign;
    return (*this);
}

Server::Server(char *port_str, char *password_str)
{
    this->_PortNumber = parsePortNumber(port_str);
    this->_ServerPassword = parsePassword(password_str);
    _ServerSocket = create_server_socket(_PortNumber);

}

int Server::parsePortNumber(char *port_str)
{
    int port = std::atoi(port_str);
    if (port < 1024 || port > 65535)
        throw(std::runtime_error("Enter a number between 1024 and 65535 to be sure, that it is not already in use"));
    return port;
}

std::string Server::parsePassword(char *password_str)
{
    std::string pass(password_str);
    return pass;
}

void set_non_blocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

int Server::create_server_socket(int port)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0)
    {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    set_non_blocking(server_socket);
    return server_socket;
}


void Server::accept_create_new_client()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(_ServerSocket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket >= 0)
    {
        set_non_blocking(client_socket);
        _clients.push_back(client_socket);
        std::cout << "New client connected: " << client_socket << std::endl;
    }
}


void Server::handle_client_disconnection(int client_socket)
{
    close(client_socket);
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client_socket), _clients.end());
    std::cout << "Client disconnected: " << client_socket << std::endl;
}


void Server::handle_client_data(int client_socket)
{
    char buffer[512];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        handle_client_disconnection(client_socket);
        return;
    }
    buffer[bytes_received] = '\0';
    std::string message(buffer);
    std::cout << "Received message from " << client_socket << ": " << message << std::endl;
    for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it != client_socket)
        {
            send(*it, message.c_str(), message.size(), 0);
        }
    }
}


int main() {
    int port = 6667;  // Default IRC port
    int server_socket = create_server_socket(port);
    set_non_blocking(server_socket);

    std::vector<int> clients;

    while (true) {
        struct pollfd fds[clients.size() + 1]; // +1 for the server socket
        int num_fds = 0;

        // Add server socket to poll set
        fds[num_fds].fd = server_socket;
        fds[num_fds].events = POLLIN;
        num_fds++;

        // Add client sockets to poll set
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it) {
            fds[num_fds].fd = *it;
            fds[num_fds].events = POLLIN;
            num_fds++;
        }

        // Call poll to wait for events
        int timeout = 1000; // 1 second timeout
        int activity = poll(fds, num_fds, timeout);
        if (activity < 0 && errno != EINTR) {
            perror("Poll error");
            break;
        }

        // Check for events on server socket
        if (fds[0].revents & POLLIN) {
            accept_new_client(server_socket, clients);
        }

        // Check for events on client sockets
        for (int i = 1; i < num_fds; ++i) {
            if (fds[i].revents & POLLIN) {
                handle_client_data(fds[i].fd, clients);
            }
        }
    }

    close(server_socket);
    return 0;
}

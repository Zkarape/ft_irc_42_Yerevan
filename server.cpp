#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

int main()
{
    int server_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socketfd == -1)
    {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999);

    if (bind(server_socketfd, (struct sockaddr *)&sockaddr,
         sizeof(sockaddr)) < 0)
    {
        std::cout << "Failed to bind socket!\n";
        close(server_socketfd);
        return 1;
    }
    std::cout << "Listening with protocol TCP on port" << port << ".\n";

    if (listen(socketdesc, 3) < 0)
    {
        std::cout << "Failed to listen!\n";
        close(server_socketfd);
        return 1;
    }

    int new_socket;
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Failed to accept connection." << std::endl;
            close(server_fd);
            return -1;
        }

        char buffer[1024] = {0};
        const char* hello = "Hello from server";

        int valread = read(new_socket, buffer, 1024);
        std::cout << "Received: " << buffer << std::endl;

        send(new_socket, hello, strlen(hello), 0);
        std::cout << "Hello message sent" << std::endl;

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
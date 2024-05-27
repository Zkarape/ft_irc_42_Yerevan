#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <cstdlib>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>


class Server
{
private:
    int _PortNumber;
    int _ServerSocket;
    std::string _ServerPassword;
    int parsePortNumber(char *port_str);
    std::string parsePassword(char *password_str);
    // void set_non_blocking(int socket);
    int create_server_socket(int port);
    void accept_create_new_client();
    void handle_client_data(int client_socket);
    void handle_client_disconnection(int client_socket);
    std::vector<int> _clients;
public:
    Server();
    Server(char *port_str, char *password_str);
    Server(const Server &copy);
    Server &operator=(const Server &assign);
    ~Server();
};

#endif
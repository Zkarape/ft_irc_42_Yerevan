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
    int _NumberOfPollfds;
    std::string _ServerPassword;
    std::string parsePassword(char *passwordStr);
    std::vector<int> _clients;
    std::vector<pollfd> _Pollfds;
    int parsePortNumber(char *portStr);
    int create_server_socket(int port);
    int call_poll_to_wait_for_events();
    void accept_create_new_client();
    void handle_client_data(int clientSocket);
    void handle_client_disconnection(int clientSocket);
    void add_sockets_to_poll_set();
public:
    Server();
    Server(const Server &copy);
    Server(char *portStr, char *passwordStr);
    Server &operator=(const Server &assign);
    void main_loop();
    ~Server();
};

#endif
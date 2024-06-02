#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <cstdlib>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include "User.hpp"
#include "Channel.hpp"

class Server
{
private:
    User _user; // Use pointer or reference to avoid incomplete type issues
    int _PortNumber;
    int _ServerSocket;
    int _NumberOfPollfds;
    int _maxfd;
    int _selectFd;
    // fd_set _readFds;
    // fd_set _writeFds;
    fd_set _masterReadFds;
    fd_set _masterWriteFds;
    std::string _ServerPassword;
    std::string _msgSendToClient;
    std::map<int, User> _clients;
    struct sockaddr_in _clientAddr;
    socklen_t _clientLen;
    // std::vector<pollfd> _Pollfds;
    // std::map<std::string, Channel *> _channels;
    std::string parsePassword(char *passwordStr);
    int parsePortNumber(char *portStr);
    int create_server_socket(int port);
    int call_poll_to_wait_for_events();
    int accept_create_new_client();
    void handle_client_data(int clientSocket);
    void handle_client_disconnection(int clientSocket);
    void add_sockets_to_poll_set();
    void selectCall();
    void addToReadSet(int fd);
    void addToWriteSet(int fd);
    void recv_part(int client_fd, fd_set& copy_of_master_read_set, int selectFd);
    void send_part();
public:
    Server();
    Server(const Server &copy);
    Server(char *portStr, char *passwordStr);
    Server &operator=(const Server &assign);
    void main_loop();
    ~Server();
};

#endif
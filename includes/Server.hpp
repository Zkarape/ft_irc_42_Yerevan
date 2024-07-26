#ifndef SERVER_HPP
# define SERVER_HPP

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
#include "Command.hpp"

class Command;
class User;
class Channel;
class Server
{
private:
    User *_user; // Use pointer or reference to avoid incomplete type issues
    int _PortNumber;
    int _ServerSocket;
    int _NumberOfPollfds;
    int _maxfd;
    int _selectFd;
    std::string _ServerPassword;
    std::string _msgSendToClient;
    std::map<int, User *> _clients;
    std::map<std::string, Command *> _commands;
    std::map<std::string, Channel *> _channels;
    std::vector<User *> _clientToDelete;
    struct sockaddr_in _clientAddr;
    socklen_t _clientLen;
    std::string parsePassword(char *passwordStr);
    void addClientToDelete(User *client);
    int parsePortNumber(char *portStr);
    int create_server_socket(int port);
    int accept_create_new_client();
    void deleteClient(User *);
    int handle_client_data(fd_set &read_fds, fd_set &write_fds);
    // void handle_client_disconnection(int clientSocket);
    void selectCall(fd_set &, fd_set &);
    void print_channels();
    int recv_part(int client_fd, fd_set &copy_of_master_read_set, User *user);
    int send_part(int client_fd, User *user);
    void to_delete_clients();
public:
    Server();
    Server(const Server &copy);
    Server(char *portStr, char *passwordStr);
    std::string getPASS(void) const;
    Server &operator=(const Server &assign);
    User* getClient(const std::string& nick);
    Channel* getChannel(const std::string& name);
    Channel* createChannel(const std::string& name, const std::string& pass, User &client);
    void delChannel(Channel *channel);
    void checkForCloseCannel(void);
    void addChannel(Channel &channel);
    void main_loop();
    ~Server();
};

#endif
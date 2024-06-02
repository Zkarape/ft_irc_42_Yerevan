#ifndef USER_HPP
#define USER_HPP

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

class User
{
private:
    int _Socket;
    bool is_admin;
    sockaddr_in _address;
    std::string _Nick;
    std::string _realname;
    std::string _message;
    std::string _input;
    std::string _command;
    std::vector<std::string> _nicknames;
    // std::map<std::string, Channel> _map_for_channels_that_user_belongs_to;
    // std::map<std::string, void (*)(Command *)> command_function;
public:
    User();
    User(int fd);
    User(const User &copy);
    User &operator=(const User &copy);
    void setInput(char *);
    void splitAndAssign();
    ~User();
};

#endif
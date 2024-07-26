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
#include "Command.hpp"
#include "EventManager.hpp"

class Command;
class Channel;


enum TypeClient
{
    Admin,
    Operator,
    Primary
};

class User
{
private:
    int _Socket;
    bool is_admin;
    bool _registered;
    struct sockaddr _address;
    std::string _args;
    std::string _Nick;
    std::string _password;
    std::string _username;
    std::string _realname;
    std::string _message;
    std::string _input;
    std::string _command;
    std::string _MsgToBeSent;
    std::string _finalResponse;
    std::vector<std::string> _nicknames;
    std::map<std::string, std::pair<Channel*, TypeClient> > _channels;
    // std::map<std::string, Channel> _map_for_channels_that_user_belongs_to;
    // std::map<std::string, void (*)(Command *)> command_function;
public:
    User();
    User(int fd, const struct sockaddr &addr);
    User(int fd);
    User(const User &copy);
    bool _isColon;
    int getFd(void);
    int splitAndAssign();
    bool isRegistered(void);
    std::string _buffer;
    User &operator=(const User &copy);
    std::string getCommand(void) const;
    std::string getPassword(void) const;
    std::string getNickname(void) const;
    std::string getMsgToBeSent() const;
    std::string getPrefix(void) const;
    std::string getMSG(void) const;
    std::vector<std::string> getNicknamesAsArguments(void) const;
    bool checkForRegistered(void);
    void setNickname(const std::string& nick);
    void setUSER(const std::string& username, const std::string& realname);
    void leaveChannel(const std::string &name);
    void setPassword(const std::string& pass);
    void joinToChannel(Channel &channel);
    void leaveALLChannels();
    void appendResponse(const std::string &str);
    void sendMsg(const std::string& msg);
    void reply(const std::string& reply);
    void sendMsgToBeSent(void);
    void setInput(char *);
    void setInput(std::string);

    ~User();
};

#endif
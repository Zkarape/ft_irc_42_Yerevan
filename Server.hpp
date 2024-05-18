#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <map>
#include <cstdlib>
#include <arpa/inet.h>
#include <vector>
#include <poll.h>

class Server
{
    private:
        int _PortNumber;
        std::string _ServerPassword;
        int parsePortNumber(char *port_str);
        std::string parsePassword(char *password_str);
    public:
        Server();
        Server(char *port_str, char *password_str);
        Server(const Server &copy);
        Server &operator=(const Server &assign);
        ~Server();
};

#endif

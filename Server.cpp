#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

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
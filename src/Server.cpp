#include "../includes/Server.hpp"

Server::Server() {}

Server::~Server()
{
    close(_ServerSocket);
    // for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    // {
    //     close(*it);
    // }
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

Server::Server(char *port_str, char *password_str) : _NumberOfPollfds(0)
{
    this->_PortNumber = parsePortNumber(port_str);
    this->_ServerPassword = parsePassword(password_str);
    _ServerSocket = create_server_socket(_PortNumber);
    _maxfd = _ServerSocket;
    _clientLen = sizeof(_clientAddr);
    FD_ZERO(&_masterReadFds);
    FD_ZERO(&_masterWriteFds);
    addToReadSet(_ServerSocket);
}

void Server::addToReadSet(int fd)
{
    FD_SET(fd, &_masterReadFds);
}

void Server::addToWriteSet(int fd)
{
    FD_SET(fd, &_masterWriteFds);
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
    if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
    {
        close(socket);
        std::cout << "Error: setting client fd to non-blocking mode failed!" << std::endl;
    }
}

int Server::create_server_socket(int port)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        perror("setsockopt");
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

void Server::selectCall()
{
    fd_set copy_of_master_read_set = _masterReadFds;
    fd_set copy_of_master_write_set = _masterWriteFds;
    int accept_retval;
    _selectFd = select(_maxfd + 1, &copy_of_master_read_set, &copy_of_master_read_set, NULL, NULL);
    if (_selectFd == -1)
    {
        perror("select returned -1\n");
    }
    else if (_selectFd)
    {
        if (FD_ISSET(_ServerSocket, &copy_of_master_read_set))
        {
            if (accept_retval = accept_create_new_client() >= _maxfd)
            {
                _maxfd = accept_retval;
            }
            addToReadSet(accept_retval);
            User *tmp = new User(accept_retval);
            _clients[accept_retval] = tmp;
        }
    }
}

int Server::accept_create_new_client()
{
    int client_socket = accept(_ServerSocket, (struct sockaddr *)&_clientAddr, &_clientLen);
    if (client_socket >= 0)
    {
        set_non_blocking(client_socket);
        std::cout << "New client connected: " << client_socket << std::endl;
    }
    else
        perror("accept returned < 0\n");
    return (client_socket);
}

void Server::handle_client_disconnection(int client_socket)
{
    close(client_socket);
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client_socket), _clients.end());
    std::cout << "Client disconnected: " << client_socket << std::endl;
}

int Server::recv_part(int client_fd, fd_set &copy_of_master_read_set, User *user)
{
    char buffer[512];
    std::cout << "clinet_socket == " << std::endl;
    if (FD_ISSET(client_fd, &copy_of_master_read_set))
    {
        _selectFd--;
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
        {
            if (bytes_received == 0)
            {
                std::cout << "Client disconnected: " << client_fd << std::endl;
                std::map<int, User *>::iterator clientIt = _clients.find(client_fd);
                if (clientIt != _clients.end())
                {
                    this->_commands["QUIT"]->execute(*clientIt->second, clientIt->second->getNicknamesAsArguments());
                }
                printf("selectserver: socket %d hung up\n", client_fd);
            }
            else
            {
                perror("recv");
            }
            close(client_fd);
            _clients.erase(std::remove(_clients.begin(), _clients.end(), client_fd), _clients.end());
            return;
        }
        else if (bytes_received > 0)
        {
            user->_buffer.append(buffer, bytes_received);
            if (user->_buffer.find('\n') != std::string::npos)
            {
                std::cout << "_buffer = " << user->_buffer << std::endl;
                user->splitAndAssign();
                if (user->getCommand().empty() == true)
                {
                    user->_buffer.clear();
                    return (1);
                }
                std::cout << "user->getCommand() = " << user->getCommand() << std::endl;
                if (_commands.find(user->getCommand()) == _commands.end())
                {
                    // user->reply(ERR_UNKNOWNCOMMAND(user->getNickname(), user->getCommand()));
                    user->_buffer.clear();
                    return 1;
                }
                user->_buffer.clear();
                this->_commands[user->getCommand()]->execute(*user, user->getNicknamesAsArguments());
            }
        }
    }
}

int Server::send_part(int client_fd, fd_set &copy_of_master_read_set, User *user)
{
    user->sendMsgToBeSent();
    delFromWriteSet(client_fd);
}

void Server::addClientToDelete(User *client)
{
    this->_clientToDelete.push_back(client);
}

void Server::handle_client_data()
{
    for (std::map<int, User *>::iterator it = this->_clients.begin(); _selectFd != 0 && it != this->_clients.end(); ++it)
    {
        if (FD_ISSET(it->first, &_masterReadFds))
        {
            recv_part(it->first, _masterReadFds, it->second);
        }
        else if (FD_ISSET(it->first, &_masterWriteFds))
        {
            send_part(it->first, _masterWriteFds, it->second);
        }
    }
    // for (size_t i = 0; i < _clientToDelete.size(); ++i)
    // {
    //     std::cout << " _clientToDelete[i]->getNICK()" << _clientToDelete[i]->getNICK() << std::endl;
    //     ;
    //     deleteClient(_clientToDelete[i]);
    // }
    // _clientToDelete.clear();
}

void Server::delFromWriteSet(int fd)
{
    FD_CLR(fd, &_masterWriteFds);
}

void Server::delFromReadSet(int fd)
{
    FD_CLR(fd, &_masterReadFds);
}

void Server::print_channels()
{
    std::map<std::string, Channel *>::iterator it = this->_channels.begin();
    for( ; it != this->_channels.end(); ++it)
    {
        std::cout << "!!!: " << it->first << std::endl;
    }
}

void Server::main_loop()
{
    while (true)
    {
        selectCall();
        handle_client_data();
    }
    close(_ServerSocket);
}
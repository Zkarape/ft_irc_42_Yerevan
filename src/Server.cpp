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

Server& Server::operator=(const Server& assign) {
    if (this != &assign) {
        // Deep copy _user
        if (_user) {
            delete _user;
        }
        if (assign._user) {
            _user = new User(*assign._user);
        } else {
            _user = NULL;
        }

        // Copy simple data members
        _PortNumber = assign._PortNumber;
        _ServerSocket = assign._ServerSocket;
        _NumberOfPollfds = assign._NumberOfPollfds;
        _maxfd = assign._maxfd;
        _selectFd = assign._selectFd;
        _ServerPassword = assign._ServerPassword;
        _msgSendToClient = assign._msgSendToClient;
        _clientAddr = assign._clientAddr;
        _clientLen = assign._clientLen;

        // Deep copy _clients map
        for (std::map<int, User*>::const_iterator it = assign._clients.begin(); it != assign._clients.end(); ++it) {
            _clients[it->first] = new User(*it->second);
        }

        // // Deep copy _commands map
        // for (std::map<std::string, Command*>::const_iterator it = assign._commands.begin(); it != assign._commands.end(); ++it) {
        //     _commands[it->first] = new Command(*it->second);
        // }

        // Deep copy _channels map
        for (std::map<std::string, Channel*>::const_iterator it = assign._channels.begin(); it != assign._channels.end(); ++it) {
            _channels[it->first] = new Channel(*it->second);
        }

        // Deep copy _clientToDelete vector
        for (std::vector<User*>::const_iterator it = assign._clientToDelete.begin(); it != assign._clientToDelete.end(); ++it) {
            _clientToDelete.push_back(new User(**it));
        }
    }
    return *this;
}

Server::Server(char *port_str, char *password_str)
{
    this->_PortNumber = parsePortNumber(port_str);
    this->_ServerPassword = parsePassword(password_str);
    _ServerSocket = create_server_socket(_PortNumber);
    EventManager::start();
    EventManager::addReadFd(_ServerSocket);
    _maxfd = _ServerSocket;
    _clientLen = sizeof(_clientAddr);

    _commands["PASS"] = new Pass(*this, false);
    _commands["NICK"] = new Nick(*this, false);
    _commands["USER"] = new UserCmd(*this, false);
    // _commands["QUIT"] = new Quit(*this, false);

    _commands["PING"] = new Ping(*this);
    _commands["PONG"] = new Pong(*this);
    _commands["JOIN"] = new Join(*this);
    _commands["KICK"] = new Kick(*this);
    _commands["MODE"] = new Mode(*this);
    _commands["INVITE"] = new Invite(*this);

	_commands["PRIVMSG"] = new PrivMsg(*this);
    _commands["TOPIC"] = new Topic(*this);
    // _commands["CAP"] = new Cap(*this);
    _commands["NOTICE"] = new Notice(*this);
    _commands["WHO"] = new Who(*this);

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


User* Server::getClient(const std::string& nick)
{
    std::map<int, User *>::iterator it_b = _clients.begin();
    std::map<int, User *>::iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (!nick.compare(it_b->second->getNickname()))
            return it_b->second;

        it_b++;
    }

    return (NULL);
}


Channel* Server::getChannel(const std::string& name)
{
    std::map<std::string, Channel *>::iterator it = this->_channels.find(name);
    if (it != this->_channels.end())
    {
        return(it->second);
    }
    return (NULL);
}


std::string Server::getPASS(void) const
{
    return _ServerPassword;
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
    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
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

void Server::selectCall(fd_set &copy_of_master_read_set, fd_set &copy_of_master_write_set)
{
    int accept_retval;
    std::cout << "asads" << std::endl;
    _selectFd = select(_maxfd + 1, &copy_of_master_read_set, &copy_of_master_write_set, NULL, NULL);
    std::cout << _selectFd << "asads" << std::endl;
    if (_selectFd == -1)
    {
        perror("select returned -1\n");
    }
    else if (_selectFd)
    {
        if (FD_ISSET(_ServerSocket, &copy_of_master_read_set))
        {
            if ((accept_retval = accept_create_new_client()) >= _maxfd)
            {
                _maxfd = accept_retval;
            }
            EventManager::addReadFd(accept_retval);
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


void Server::checkForCloseCannel(void)
{
    std::vector<std::string> closedFds; 
    std::map<std::string, Channel *>::iterator it = _channels.begin();

    for ( ; it != _channels.end(); ++it)
    {
        if ((it->second)->emptyClients())
        {
            closedFds.push_back(it->first);
        }
    }

    for (size_t i = 0; i < closedFds.size(); ++i)
    {
        delete _channels[closedFds[i]];
        _channels.erase(closedFds[i]);
    }
}

void Server::delChannel(Channel *channel)
{
    if (channel)
    {
        this->_channels.erase(channel->_name);
        delete channel;
    }
}


Channel* Server::createChannel(const std::string& name, const std::string& pass, User &client)
{
    Channel *new_channel = new Channel(name, pass, &client);
    this->addChannel(*new_channel);
    return (new_channel);
}

void Server::addChannel(Channel &channel)
{
    if (this->_channels.insert(std::make_pair(channel._name, &channel)).second == false)
    {
        std::cout << "alredy exist\n";
    }
}


// void Server::handle_client_disconnection(int client_socket)
// {
//     close(client_socket);
//     _clients.erase(std::remove(_clients.begin(), _clients.end(), client_socket), _clients.end());
//     std::cout << "Client disconnected: " << client_socket << std::endl;
// }

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
            _clients.erase(client_fd);
            // _clients.erase(std::remove(_clients.begin(), _clients.end(), client_fd), _clients.end());
            return 1;
        }
        else if (bytes_received > 0)
        {
            user->_buffer.append(buffer, bytes_received);
            if (user->_buffer.find('\n') != std::string::npos)
            {
                std::cout << "_buffer = " << "++" << user->_buffer << "++" << std::endl;
                user->setInput(user->_buffer);
                if (user->splitAndAssign())
                {
                    return 1;
                }
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
    return 0;
}

int Server::send_part(int client_fd, User *user)
{
    user->sendMsgToBeSent();
    EventManager::delWriteFd(client_fd);
    return 1;
}

void Server::addClientToDelete(User *client)
{
    this->_clientToDelete.push_back(client);
}

void Server::deleteClient(User *client)
{
    this->_clients.erase(client->getFd());
    delete client;
    this->_clients.erase(100);
}


void Server::to_delete_clients()
{
    for (size_t i = 0; i < _clientToDelete.size(); ++i)
    {
        std::cout << " _clientToDelete[i]->getNICK()" << _clientToDelete[i]->getNickname() << std::endl;
        deleteClient(_clientToDelete[i]);
    }
    _clientToDelete.clear();
}


int Server::handle_client_data(fd_set &read_fds, fd_set &write_fds)
{
    EventManager::addReadFd(this->_ServerSocket);
    for (std::map<int, User *>::iterator it = this->_clients.begin(); _selectFd != 0 && it != this->_clients.end(); ++it)
    {
        if (FD_ISSET(it->first, &read_fds))
        {
            if (recv_part(it->first, read_fds, it->second) == 1)
                return 1;
        }
        else if (FD_ISSET(it->first, &write_fds))
        {
            if (send_part(it->first, it->second) == 1)
                return 1;
        }
        to_delete_clients();
    }
    return 1;
}

void Server::print_channels()
{
    std::map<std::string, Channel *>::iterator it = this->_channels.begin();
    for (; it != this->_channels.end(); ++it)
    {
        std::cout << "!!!: " << it->first << std::endl;
    }
}

void Server::main_loop()
{
    while (true)
    {
        fd_set read_fds = *EventManager::getReadFdSet(); // copy it
        fd_set write_fds = *EventManager::getWriteFdSet(); // copy it
        selectCall(read_fds, write_fds);
        std::cout << "SIZE AFTER SELECT" << std::endl;
        if (handle_client_data(read_fds, write_fds) == 1)
            continue;
    }
    close(_ServerSocket);
}
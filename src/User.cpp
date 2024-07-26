#include "../includes/User.hpp"

User::User() : is_admin(0), _Nick("Default_nickname") {}

User::User(const User &copy)
{
    this->_Socket = copy._Socket;
    this->is_admin = copy.is_admin;
    this->_Nick = copy._Nick;
}

User::User(int fd, const struct sockaddr &addr) : _Socket(fd)
{
    this->_address = addr;
    this->_registered = false;

    char hostname[NI_MAXHOST];
    getnameinfo((struct sockaddr*)&addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    // _hostname = hostname;
}

int User::getFd(void)
{
    return (this->_Socket);
}


std::string User::getPrefix(void) const
{
    std::string prefix = this->_Nick;

    if (!this->_username.empty())
    {
        prefix += "!" + this->_username;
    }

    if (!this->_realname.empty())
    {
        prefix += "@" + this->_realname;
    }

    return (prefix); 
}


void User::reply(const std::string& reply)
{
    std::string buff = ":" + this->getPrefix() + " " + reply + "\r\n";

    EventManager::addWriteFd(this->_Socket);
    this->appendResponse(buff);
}


void User::appendResponse(const std::string &str)
{
    _finalResponse.append(str);
};


void User::sendMsg(const std::string& msg)
{
    std::string buff = msg + "\r\n";

    EventManager::addWriteFd(this->_Socket);
    this->appendResponse(buff);
}


void User::sendMsgToBeSent(void)
{
    if (send(this->_Socket, this->_MsgToBeSent.c_str(), this->_MsgToBeSent.length(), 0) < 0)
        std::cerr << "Error: can't send message to client." << std::endl;
    this->_MsgToBeSent.clear();
}

std::string User::getMsgToBeSent() const
{
    return _MsgToBeSent;
}

User::User(int fd) : _Socket(fd) {}

std::string User::getCommand(void) const
{
    return _command;
}

std::vector<std::string> User::getNicknamesAsArguments(void) const
{
    return _nicknames;
}

User &User::operator=(const User &assign)
{
    *this = assign;
    return *this;
}

void User::setInput(char *input)
{
    _input = input;
}

void User::setInput(std::string input)
{
    _input = input;
}

std::string User::getNickname(void) const
{
    return _Nick;
}

std::string User::getMSG(void) const
{
    return (_message);
}

bool User::isRegistered(void)
{
    return (this->_registered);
}

void User::setNickname(const std::string& nick)
{
    this->_Nick = nick;
}

void User::setPassword(const std::string& pass)
{
    this->_password = pass;
}

std::string User::getPassword(void) const
{
    return _password;
}

void User::setUSER(const std::string& username, const std::string& realname)
{
    this->_username = username;
    this->_realname = realname;
}


bool User::checkForRegistered(void)
{
    if (!_password.empty() && !_username.empty() && !_Nick.empty() && !_registered)
    {
        this->_registered = true;
        reply(RPL_WELCOME(_Nick));
    }
    else
        this->_registered = false;

    return (this->_registered);
}

void User::leaveChannel(const std::string &name)
{
    std::map<std::string, std::pair<Channel*, TypeClient> >::iterator it = this->_channels.find(name);

    if (it != this->_channels.end())
    {
        it->second.first->deleteClient(*this);
        this->_channels.erase(it);
    }
}


int User::splitAndAssign()
{
    // Find the first space to extract the command
    size_t firstSpace = _input.find(' ');
    std::cout << _input << " " << firstSpace << "=--------\n";
    if (firstSpace == std::string::npos)
    {
        std::cerr << "Invalid input format: no command found" << std::endl;
        return 1;
    }

    // Extract the command
    _command = _input.substr(0, firstSpace);

    // Find the colon to separate nicknames from the message
    size_t colonPos = _input.find(':', firstSpace + 1);
    if (colonPos == std::string::npos)
    {
        std::cerr << "Invalid input format: no message found" << std::endl;
        return 1;
    }

    // Extract the nicknames substring
    std::string nicknamesStr = _input.substr(firstSpace + 1, colonPos - firstSpace - 2);

    // Extract the message
    _message = _input.substr(colonPos + 1);

    // Split the nicknames by commas
    std::stringstream ss(nicknamesStr);
    std::string nickname;
    while (std::getline(ss, nickname, ' '))
    {
        this->_nicknames.push_back(nickname);
    }
    std::cout << "_nicknames.size() == " << _nicknames.size() << std::endl;
    if (_nicknames.size() != 1)
    {
        std::cerr << "Invalid input format: no message found" << std::endl;
        return 1;
    }
    // while (std::getline(ss, nickname, ','))
    // {
    //     this->_nicknames.push_back(nickname);
    // }
    return 1;
}


void User::joinToChannel(Channel &channel)
{

    std::map<std::string, std::pair<Channel*, TypeClient> >::iterator it = this->_channels.find(channel.getName());

    if (it == this->_channels.end())
    {
        this->_channels.insert(std::pair<std::string, std::pair<Channel*, TypeClient> >(channel.getName(), std::pair<Channel*, TypeClient>(&channel, Primary)));
    }
}

void User::leaveALLChannels()
{
    std::map<std::string, std::pair<Channel*, TypeClient> >::iterator it = this->_channels.begin();

    for (; it != this->_channels.end(); ++it)
    {
        it->second.first->deleteClient(*this);
    }
    this->_channels.clear();
}


User::~User()
{
    this->leaveALLChannels();
    close(_Socket);
    EventManager::delReadFd(_Socket);
    EventManager::delWriteFd(_Socket);
}
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
    // _address = addr;
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

std::string User::getNickname(void) const
{
    return _Nick;
}

void User::splitAndAssign()
{
    // Find the first space to extract the command
    size_t firstSpace = _input.find(' ');
    if (firstSpace == std::string::npos)
    {
        std::cerr << "Invalid input format: no command found" << std::endl;
        return;
    }

    // Extract the command
    _command = _input.substr(0, firstSpace);

    // Find the colon to separate nicknames from the message
    size_t colonPos = _input.find(':', firstSpace + 1);
    if (colonPos == std::string::npos)
    {
        std::cerr << "Invalid input format: no message found" << std::endl;
        return;
    }

    // Extract the nicknames substring
    std::string nicknamesStr = _input.substr(firstSpace + 1, colonPos - firstSpace - 2);

    // Extract the message
    _message = _input.substr(colonPos + 1);

    // Split the nicknames by commas
    std::stringstream ss(nicknamesStr);
    std::string nickname;
    while (std::getline(ss, nickname, ','))
    {
        this->_nicknames.push_back(nickname);
    }
}

User::~User() {}
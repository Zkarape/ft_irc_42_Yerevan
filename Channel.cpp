#include "Channel.hpp"

Channel::Channel(std::string &name, std::string &pswd, User *owner)
{
    this->_name = name;
    this->_pswd = pswd;
    this->_owner = owner;
    this->_invite_only = false;
    this->_limit = 0;
}

std::string Channel::getName()
{
    return (this->_name);
}

std::string Channel::setPswd()
{
    return (this->_pswd);
}

std::string Channel::setTopic()
{
    return (this->_topic);
}

std::string Channel::setMode()
{
    return (this->_mode);
}

std::vector<User *> Channel::getUsers()
{
    return (this->_users);
}

User *Channel::getUser(const std::string &nickname)
{
    for (std::vector<User *>::iterator it = _users.begin(); it < _users.end(); it++)
    {
        if (*(it)->getNickname() == nickname) //// USER should have getNickname getter
            return (*it);
    }
    return (NULL);
}

User *Channel::getOwner()
{
    return (this->_owner);
}

int *Channel::getLimit()
{
    return (this->_limit);
}

void Channel::setName(const std::string &name)
{
    _name = name;
}

void Channel::setPswd(const std::string &pswd)
{
    _pswd = pswd;
}

void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

void Channel::setMode(const std::string &mode)
{
    _mode = mode;
}

void Channel::setOwner(User *owner)
{
    _owner = owner;
}

void Channel::setInvite(User *user)
{
    _invitees.push_back(user);
}

void Channel::sendMsgToChannel(User *user, const std::string &msg)
{
    for (std::vector<User *>::iterator it = _users.begin(); it < _users.end(); it++)
    {
        if (*it != user)
            (*it)->_owner(msg);
    }
}

void Channel::setOperator()
{
    std::string nickname = this->_mode.substr(2);
    User *user = this->getUser(nickname);
    if (user)
    {
        _operators.push_back(user);
        this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+o " + user->getNickname()));
        _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+o " + user->getNickname()));
    }
    else
    {
        this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-o " + nickname));
        _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-o " + nickname));
    }
}

void Channel::unsetOperator()
{
    std::string nickname = _mode.substr(2);
    User *user = this->getUser(nickname);
    if (user)
    {
        for (std::vector<User *>::iterator it = _operators.begin(); it != _operators.end(); ++it)
        {
            if (*it == user)
            {
                _operators.erase(it);
                break;
            }
        }
    }
    this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-o " + nickname));
    _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-o " + nickname));
}

void Channel::unsetInvite(User *user)
{
    for (std::vector<User *>::iterator it = _invitees.begin(); it != _invitees.end(); it++)
    {
        if (*it == user)
        {
            _invitees.erase(it);
            break;
        }
    }
}

void Channel::applyMode()
{
    if (_mode[0] == '+')
    {
        if (_mode[1] == 'i')
        {
            _invite_only = true;
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+i"));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+i"));
        }
        else if (_mode[1] == 't')
        {
            _topic_operators = true;
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+t"));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+t"));
        }
        else if (_mode[1] == 'k')
        {
            _pswd = _mode.substr(2);
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+k " + _pswd));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+k " + _pswd));
        }
        else if (_mode[1] == 'o')
        {
            this->setOperator();
        }
        else if (_mode[1] == 'l')
        {
            std::string limit = _mode.substr(2);
            for (std::string::iterator it = limit.begin(); it != limit.end(); ++it)
            {
                if (!isdigit(*it))
                {
                    this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-l"));
                    _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-l"));
                    return;
                }
            }

            _limit = atoi(limit.c_str());
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+l " + std::to_string(_limit)));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "+l " + std::to_string(_limit)));
        }
    }
    else if (_mode[0] == '-')
    {
        if (_mode[1] == 'i')
        {
            _invite_only = false;
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-i"));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-i"));
        }
        else if (_mode[1] == 't')
        {
            _topic_operators = false;
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-t"));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-t"));
        }
        else if (_mode[1] == 'k')
        {
            _pswd = "";
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-k"));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-k"));
        }
        else if (_mode[1] == 'o')
        {
            unsetOperator();
        }
        else if (_mode[1] == 'l')
        {
            _limit = 0;
            this->sendMsgToChannel(_owner, RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-l"));
            _owner->sendMsg(RPL_CHANNELMODEIS(_owner->getNickname(), _name, "-l"));
        }
    }
    _mode.clear();
}

void Channel::join(User *user)
{
    _users.push_back(user);
}

void Channel::part(User *user)
{
    std::vector<User *>::iterator tmp;
    for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        (*it)->sendMsg(RPL_PART(user->getMessage(), _name));
        if (*it == user)
            tmp = it;
    }
    _users.erase(tmp);
}

void Channel::kick(User *user, std::string const &msg)
{
    std::vector<User *>::iterator tmp;
    for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        (*it)->sendMsg(RPL_KICK(user->getMessage(), _name, user->getNickname(), msg));
        if (*it == user)
            tmp = it;
    }
    _users.erase(tmp);
}

bool Channel::isOwner(User *user)
{
    if (user == _owner)
        return true;
    return false;
}

bool Channel::isOperator(User *user)
{
    for (std::vector<User *>::iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        if (*it == user)
            return true;
    }
    return false;
}

bool Channel::isExist(User *user)
{
    for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (*it == user)
            return true;
    }
    return false;
}

bool Channel::isInvite(User *user)
{
    for (std::vector<User *>::iterator it = _invitees.begin(); it != _invitees.end(); ++it)
    {
        if (*it == user)
            return (true);
    }
    return (false);
}

bool Channel::isInviteOnly()
{
    return (_invite_only);
}

bool Channel::isTopicOperators()
{
    return (_topic_operators);
}

void Channel::join(User *user)
{
    _users.push_back(user);
}
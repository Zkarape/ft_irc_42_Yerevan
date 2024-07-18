#include "Command.hpp"

Topic::Topic(Server& srv) : Command(srv) {}

Topic::~Topic() {}

// syntax: Topic <channel> :[<message>]

void    Topic::execute(User& client, std::vector<std::string> args)
{
    if (!client.isRegistered())
    {
        client.reply(ERR_NOTREGISTERED(client.getNickname()));
        return ;
    }

    if (args.empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
        return ;
    }

    std::string channelName = args[0];
    Channel* channel = _srv.getChannel(channelName);
    if (!channel)
    {
        client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), channelName + static_cast<char>(1)));
        return ;
    }
    if (!channel->isInChannel(client))
    {
        client.reply(ERR_NOTONCHANNEL(client.getNickname(), channelName + static_cast<char>(1)));
        return ;
    }

    if (!channel->isOperator(client) && !channel->isAdmin(client))
    {
        client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName + static_cast<char>(1)));
        return ;
    }

    if (channel->isTopicModeOn() == false)
    {
        client.sendMsg(ERR_NOCHANMODES(channelName));
        return;
    }

    if (args.size() == 1)
    {
        if (client._isColon)
        {
            channel->_topic = client.getMSG();
            if (channel->_topic.empty())
            {
                client.sendMsg(RPL_NOTOPIC(channelName) + static_cast<char>(1));
            }
            else 
            {
                client.sendMsg(RPL_TOPIC(channelName + static_cast<char>(1), channel->_topic));
            }
        } 
        else
        {
            if (channel->_topic.empty())
            {
                client.sendMsg("channel topic is empty");
            }
            else 
            {
                client.sendMsg("channel topic is: " + channel->_topic);
            }
        }
    }
    else
    {
        client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
    }
}
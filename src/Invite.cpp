#include "Command.hpp"

Invite::Invite(Server& srv) : Command(srv) {}

Invite::~Invite() {}

void Invite::execute(User& client, std::vector<std::string> args)
{
    if (!client.isRegistered())
    {
        client.reply(ERR_NOTREGISTERED(client.getNickname()));
        return ;
    }

    if (args.size() < 2)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "WHO"));

        return ;
    }

    std::string nickName = args[0];
    std::string channelName = args[1];

    User* client_inv = _srv.getClient(nickName);
    if (!client_inv)
    {
        client.reply(ERR_NOSUCHNICK(client.getNickname(), nickName));
        return ;
    }


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

    if (channel->isInChannel(*client_inv))
    {
        client.reply(ERR_USERONCHANNEL(client.getNickname(), nickName, channelName + static_cast<char>(1)));
        return ;
    }

    if (channel->channelIsFull())
    {
        client.reply(ERR_CHANNELISFULL(client.getNickname(), channelName + static_cast<char>(1)));
        return ;
    }

    channel->joinClient(*client_inv);
}
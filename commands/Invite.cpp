#include "Command.hpp"

Inite::Inite(Server* srv) : Command(srv) {}

Inite::~Inite() {}

void Invite::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
         client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "WHO"));
         return:
    }

    std::string nickname = args[0];
    std::string channelName = args[1];

    Channel* channel = _srv->get_channel(channelName);
   
    if (!channel)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }
    Client *invite = _srv->get_client(nickname);
     
     if (!invite) 
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), nickname));
        return;
    }

    if (!channel->is_operator(client))
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channelName));
        return;
    }

     if (channel->is_user(invite))
     {
        client->reply(ERR_USERONCHANNEL(client->get_nickname(), nickname, channelName));
        return;
    }

    invite->reply(":" + client->get_prefix() + " INVITE " + invite->get_nickname() + " :" + channelName);
    client->reply(RPL_INVITING(client->get_nickname(), nickname, channelName));
}
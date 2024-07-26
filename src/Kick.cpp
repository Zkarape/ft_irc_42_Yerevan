#include "Command.hpp"

Kick::Kick(Server& srv) : Command(srv) {}

Kick::~Kick() {}

// syntax: KICK <channel> <client> :[<message>]

void    Kick::execute(User& client, std::vector<std::string> args)
{
    if (!client.isRegistered())
    {
        client.reply(ERR_NOTREGISTERED(client.getNickname()));
        return ;
    }

    if (args.size() < 2)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
        return ;
    }

    std::vector<std::string>::iterator it = std::find(args.begin(), args.end(), "");
    std::vector<std::string> channelNames(args.begin(), it);

    std::vector<std::string> chanelUsers;
    if (it != args.end())
        chanelUsers.insert(chanelUsers.begin(), it + 1, args.end()) /* = std::vector<std::string>(it + 1, args.end()) */;

    std::string comment = client.getMSG();
    
    std::string channelName = channelNames[0];

    Channel* channel = _srv.getChannel(channelName);
    if (!channel)
    {
        client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), channelName + static_cast<char>(1)));
        return ;
    }

    if ((!channel->isOperator(client) && !channel->isAdmin(client)))
    {
        client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName + static_cast<char>(1)));
        return ;
    }

    for( size_t i = 0; i < chanelUsers.size(); ++i)
    {
        std::string nickname = chanelUsers[i];


        User* clientKick = channel->getClienByItstNick(nickname);
        if (!clientKick)
        {
            client.reply(ERR_USERNOTINCHANNEL(client.getNickname(), nickname, channelName + static_cast<char>(1)));
            return ;
        }

        if (!channel->isInChannel(*clientKick))
        {
            client.reply(ERR_NOTONCHANNEL(client.getNickname(), channelName + static_cast<char>(1)));
            return ;
        }

        if (!(channel->isAdmin(client)) && channel->isAdmin(*clientKick))
        {
            client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName + static_cast<char>(1)));
            return ;
        }
        
        if (comment.empty())
            comment = "No reason specified.";

        channel->deleteClient(*clientKick);
        clientKick->leaveChannel(channel->getName());
        if (channel->isEmpty()) {
            
        }
        clientKick->sendMsg(RPL_KICK(channel->getAdmin().getPrefix(), channel->getName(), client.getNickname(), comment));
        if (channel->isEmpty())
        {
            _srv.delChannel(channel);
        }
    }
}
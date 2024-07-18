#include "../includes/Command.hpp"

PrivMsg::PrivMsg(Server& srv) : Command(srv) {}

PrivMsg::~PrivMsg() {}

// syntax: PRIVMSG <msgtarget> :<message>

void    PrivMsg::execute(User& client, std::vector<std::string> args)
{
    if (!client.isRegistered())
    {
        client.reply(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }
    if (args.empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
        return ;
    }

    std::string message = client.getMSG();
    if (message.empty())
    {
        client.reply(ERR_NOTEXTTOSEND(client.getNickname()));
        return ;
    }

    for (size_t i = 0; i < args.size(); i++)
    {
        std::string target = args[i];
        if (target[i] == '#' || target[i] == '&')
        {
            Channel* channel = _srv.getChannel(target);
            if (!channel)
            {
                client.reply(ERR_NOSUCHNICK(client.getNickname(), target));
                continue; ;
            }
            if (!channel->isInChannel(client))
            {
                client.reply(ERR_CANNOTSENDTOCHAN(client.getNickname(), target));
                continue; ;
            }
            
            channel->sendMsg(client, message, "PRIVMSG");
        }
        else
        {
            User* recv_client = _srv.getClient(target);
            if (!recv_client)
            {
                client.sendMsg(ERR_NOSUCHNICK(client.getNickname(), target));
                continue; ;
            }

            recv_client->sendMsg(RPL_MSG(client.getPrefix(), "PRIVMSG", target, message));

        }
    }
}
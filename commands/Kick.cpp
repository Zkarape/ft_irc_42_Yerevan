#include "Command.hpp"

Kick(Server *srv) : Command(srv) {}

Kick::~Kick() {}

void Kick::execute(User *user, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        user->reply(ERR_NEEDMOREPARAMS(user->get_nickname(), "KICK"));
        return;
    }

    std::string channelName = args[0];
    std::string nickname = args[1];
    std::string reason = "No reason specified!";

    if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() > 1))
    {
        reason = "";

        std::vector<std::string>::iterator it = args.begin() + 2;
        std::vector<std::string>::iterator end = args.end();

        while (it != end)
        {
            reason.append(*it + " ");
            it++;
        }
    }

    Channel *channel = _srv->get_channel(channelName);
    if (!channel)
    {
        user->reply(ERR_NOSUCHCHANNEL(user->get_nickname(), channelName));
        return;
    }

    if (!channel->isOperator(user))
    {
        user->reply(ERR_CHANOPRIVSNEEDED(user->get_nickname(), channelName));
        return;
    }

    User *nick = _srv->get_client(nickname);
    if (!target)
    {
        user->reply(ERR_NOSUCHNICK(user->get_nickname(), nickname));
        return;
    }

    if (!channel->isExist(nick))
    {
        user->reply(ERR_USERNOTINCHANNEL(user->get_nickname(), nickname, channelName));
        return;
    }

    channel->kick(target);
    nick->reply(":" + user->get_prefix() + " KICK " + channelName + " " + nickname + " :" + reason);
    channel->sendMsgToChannel(":" + user->get_prefix() + " KICK " + channelName + " " + nickname + " :" + reason, user);
}
#include "Command.hpp"

Invite::Invite(Server *srv) : Command(srv) {}

Invite::~Invite() {}

void Invite::execute(User *user, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        user->reply(ERR_NEEDMOREPARAMS(user->get_nickname(), "WHO"));
        return:
    }

    std::string nickname = args[0];
    std::string channelName = args[1];

    Channel *channel = _srv->get_channel(channelName);

    if (!channel)
    {
        user->reply(ERR_NOSUCHCHANNEL(user->get_nickname(), channelName));
        return;
    }
    User *invite = _srv->get_client(nickname);

    if (!invite)
    {
        user->reply(ERR_NOSUCHNICK(user->get_nickname(), nickname));
        return;
    }

    if (!channel->isOperator(user))
    {
        user->reply(ERR_CHANOPRIVSNEEDED(user->get_nickname(), channelName));
        return;
    }

    if (channel->isExist(invite))
    {
        user->reply(ERR_USERONCHANNEL(user->get_nickname(), nickname, channelName));
        return;
    }

    invite->reply(":" + user->get_prefix() + " INVITE " + invite->get_nickname() + " :" + channelName);
    user->reply(RPL_INVITING(user->get_nickname(), nickname, channelName));
}
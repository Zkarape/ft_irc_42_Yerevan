#include "Command.hpp"

Mode::Mode(Server *srv) : Command(srv) {}

Mode::~Mode() {}

void Mode::execute(User* user, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        user->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
        return;
    }
    std::string channelName = obj[0];
    std::string mode = obj[1];
    std::string target = (obj.size() == 3) ? obj[2] : "";

    Channel *channel = srv->getChannel(channelName);
    if (channel == NULL)
    {
        user->SendMsg(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return;
    }

    if (!channel->isOwner(user) && !channel->isOperator(user))
    {
        user->SendMsg(ERR_CHANOPRIVSNEEDED(user->getNickname(), channelName));
        return;
    }

    channel->setMode(mode + target);
    channel->applyMode();
}
#include "Command.hpp"

Topic::Topic(Server *srv) : Command(srv) {}
Topic::~Topic() {}

void Topic::execute(User *user, std::vector<std::string> args)
{
    if (args.size() < 1)
    {
        user->SendMsg(ERR_NEEDMOREPARAMS(user->getNickname(), "TOPIC"));
        return;
    }
    Channel *channel = _Server->get_channel(args[0]);
    if (channel == NULL)
    {
        user->SendMsg(ERR_NOSUCHCHANNEL(user->getNickname(), args[0]));
        return;
    }
    if (channel->getUser(user->getNickname()) == NULL)
    {
        user->SendMsg(ERR_NOTONCHANNEL(user->getNickname(), args[0]));
        return;
    }
    if (!channel->isAdmin(user) && !channel->isOperator(user))
    {
        user->SendMsg(ERR_CHANOPRIVSNEEDED(user->getNickname(), channel->getName()));
        return;
    }
    if (channel->isOperator(user) && !channel->isTopicOperators())
    {
        user->SendMsg(RPL_NOTOPIC(user->getNickname(), channel->getName()));
        return;
    }
    std::string topic = args[1];
    for (size_t i = 2; i < args.size(); i++)
        topic += " " + args[i];
    channel->setTopic(topic);
    user->SendMsg(RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()));
}
#include "Command.hpp"

Join::Join(Server* srv) : Command(srv) {}

Join::~Join() {}

void Join::execute(User* user, std::vector<std::string> args) 
{
    if (args.empty()) {
        user->reply(ERR_NEEDMOREPARAMS(user->get_nickname(), "JOIN"));
        return;
    }

    std::string name = args[0];
    std::string pass = args.size() > 1 ? args[1] : "";

    Channel* channel = user->get_channel(name);
    if (channel) {
        user->reply(ERR_TOOMANYCHANNELS(user->get_nickname(), name));
        return;
    }

    channel = _srv->get_channel(name);
    if (!channel)
        channel = _srv->create_channel(name, pass, user);

    if (channel->getLimit() > 0 && (int)channel->getUsers().size() >= channel->getLimit()) {
        user->reply(ERR_CHANNELISFULL(user->get_nickname(), name));
        return;
    }

    if (!channel->getPswd().empty() && channel->getPswd() != pass) {
        user->reply(ERR_BADCHANNELKEY(user->get_nickname(), name));
        return;
    }

    user->join(channel);
}

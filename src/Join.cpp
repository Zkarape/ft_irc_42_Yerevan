#include "Command.hpp"

Join::Join(Server& srv) : Command(srv) {}

Join::~Join() {}

// syntax: JOIN <channels> [<keys>]

void    Join::execute(User& client, std::vector<std::string> args)
{
    std::vector<std::string>::iterator it = std::find(args.begin(), args.end(), "");
    std::vector<std::string> channelNames(args.begin(), it);

    std::vector<std::string> chanelKeys;
    if (it != args.end())
        chanelKeys.insert(chanelKeys.begin(), it + 1, args.end()) /* = std::vector<std::string>(it + 1, args.end()) */;

    if (!client.isRegistered())
    {
        client.reply(ERR_NOTREGISTERED(client.getNickname()));
        return ;
    }

    if (args.empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
        return ;
    }

    if (args[0] == "0")
    {
        client.leaveALLChannels();
        _srv.checkForCloseCannel();
        return ;
    }
    for (size_t i = 0; i < channelNames.size(); i++)
    {
        std::string name = channelNames[i];
        std::string pass = i < chanelKeys.size() ? chanelKeys[i] : "";

        if (name[0] != '#' && name[0] != '&')
        {
            client.reply(ERR_BADCHANMASK(client.getNickname(), name + static_cast<char>(1)));
            continue;
        }

        // name.erase(0, 1);
        Channel* channel = _srv.getChannel(name);
        if (!channel)
        {
            channel = _srv.createChannel(name, pass, client);
            continue;
        }
        else  if (channel->isInChannel(client))
        {
            client.reply(ERR_USERONCHANNEL(client.getNickname(), client.getNickname(), name + static_cast<char>(1)));
            continue;
        }
        if (channel->isInviteOnly())
        {
            client.reply(ERR_INVITEONLYCHAN(client.getNickname(), name + static_cast<char>(1)));
            continue;
        }

        if (channel->channelIsFull())
        {
            client.reply(ERR_CHANNELISFULL(client.getNickname(), name + static_cast<char>(1)));
            continue;
        }

        if (channel->get_pass() != pass)
        {
            client.reply(ERR_BADCHANNELKEY(client.getNickname(), name, "Cannot join channel (+k)"));
            continue;
        }
        channel->joinClient(client);
    }
}
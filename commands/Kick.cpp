#include "Command.cpp"

Kick (Server *srv) : Command (srv) {}

Kick::~Kick() {}

void Kick::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
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
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }

    if (!channel->is_operator(client))
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channelName));
        return;
    }

    Client* nick = _srv->get_client(nickname);
    if (!target)
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), nickname));
        return;
    }

    if (!channel->is_user(nick))
    {
        client->reply(ERR_USERNOTINCHANNEL(client->get_nickname(), nickname, channelName));
        return;
    }

    channel->kick_user(target);
    nick->reply(":" + client->get_prefix() + " KICK " + channelName + " " + nickname + " :" + reason);
    channel->broadcast(":" + client->get_prefix() + " KICK " + channelName + " " + nickname + " :" + reason, client);
}
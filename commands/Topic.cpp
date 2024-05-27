#include "Command.cpp"

Topic::Topic(Server *srv) : Command(srv) {}
Topic::~Topic() {}

void Topic::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 1)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "TOPIC"));
        return;
    }

    std::string channelName = args[0];
    Channel* channel = _srv->get_channel(channelName);
    if (!channel)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }

    if (!channel->is_user(client))
    {
        client->reply(ERR_NOTONCHANNEL(client->get_nickname(), channelName));
        return;
    }

    if (args.size() == 1)
    {
        std::string topic = channel->get_topic();
        if (topic.empty())
        {
            client->reply(RPL_NOTOPIC(client->get_nickname(), channelName));
        }
        else
        {
            client->reply(RPL_TOPIC(client->get_nickname(), channelName, topic));
        }
    }

    else
    {
        if (!channel->is_operator(client) && channel->topic_protected())
        {
            client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channelName));
            return;
        }

        std::string Topic = args[1].substr(args[1][0] == ':' ? 1 : 0);
        channel->set_topic(Topic);

        channel->broadcast(RPL_TOPIC(client->get_prefix(), channelName, Topic));
    }
}
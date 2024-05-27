#include "Command.cpp"

Privmsg::Privmsg(Server* srv) : Command(srv) {}

Privmsg::~Privmsg() {}

void    Privmsg::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 2 || args[0].empty() || args[1].empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "PRIVMSG"));
        return;
    }

    std::string target = args[0];
    std::string message;

    std::vector<std::string>::iterator it = args.begin() + 1;
    std::vector<std::string>::iterator end = args.end();

    while (it != end)
    {
        message.append(*it + " ");
        it++;
    }

    if (message.at(0) == ':')
        message = message.substr(1);

    if (target.at(0) == '#')
    {
        Channel* channel = client->get_channel();

         if (!channel)
        {
            user->reply(ERR_NOSUCHNICK(user->get_nickname(), target));
            return;
        }

        if (!channel->is_member())
        {
            std::vector<std::string> nicknames = channel->get_nicknames();

            std::vector<std::string>::iterator it = nicknames.begin();
            std::vector<std::string>::iterator end = nicknames.end();

             while (it != end)
            {
                if (*it == client->get_nickname())
                    break;

                it++;
            }

            if (it == end)
            {
                client->reply(ERR_CANNOTSENDTOCHAN(client->get_nickname(), target));
                return;
            }
        }

        channel->broadcast(RPL_PRIVMSG(client->get_prefix(), target, message), client);
        return;
    }

    Client  *dest = _srv->get_client(target);
    if (!dest)
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), target));
		return;
    }

    dest->write(RPL_PRIVMSG(client->get_prefix(), target, message));
}

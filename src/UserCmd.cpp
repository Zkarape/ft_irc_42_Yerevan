#include "../includes/Command.hpp"

UserCmd::UserCmd(Server& srv, bool auth) : Command(srv, auth) {}

UserCmd::~UserCmd() {}

// stntax: USER <username> <hostname> <servername> <realname>

void    UserCmd::execute(User& client, std::vector<std::string> args)
{
    if (client.getPassword().empty())
    {
        client.reply(ERR_NOTREGISTERED(client.getNickname()));
        return ;
    }
    if (client.isRegistered())
    {
        client.reply(ERR_ALREADYREGISTERED(client.getNickname()));
        return;
    }

    if (args.size() < 4 || args[1] != "0" || args[2] != "*")
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
        return;
    }

    client.setUSER(args[0], args[3]);
    client.checkForRegistered();
}
#include "../includes/Command.hpp"

Pass::Pass(Server& srv, bool auth) : Command(srv, auth) {}

Pass::~Pass() {}

void    Pass::execute(User& client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
        return;
    }

    if (client.isRegistered())
    {
        client.reply(ERR_ALREADYREGISTERED(client.getNickname()));
        return;
    }

    if (_srv.getPASS() != args[0].substr(args[0][0] == ':' ? 1 : 0))
    {
        client.reply(ERR_PASSWDMISMATCH(client.getNickname()));
        return;
    }
    client.setPassword(args[0]);
}

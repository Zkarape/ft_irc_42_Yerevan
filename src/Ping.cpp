#include "Command.hpp"

Ping::Ping(Server& srv) : Command(srv) {}

Ping::~Ping() {}

// syntax: PING <server1> [<server2>]

void    Ping::execute(User& client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PING"));
        return;
    }
    
    client.sendMsg(RPL_PING(client.getPrefix(), args[0]));
}

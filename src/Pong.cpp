#include "Command.hpp"

Pong::Pong(Server& srv) : Command(srv) {}

Pong::~Pong() {}

// syntax: PONG <server1> [<server2>]

void    Pong::execute(User& client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PONG"));
        return;
    }
    
    client.sendMsg(RPL_PING(client.getPrefix(), args[0]));
}

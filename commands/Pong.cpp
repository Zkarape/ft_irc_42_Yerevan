#include "Command.hpp"

Pong(Server *srv) : Command(srv) {}

Pong::~Pong() {}

void Pong::execute(User *user, std::vector<std::string> args)
{
    if (args.empty())
    {
        user->ReplyMsg(ERR_NEEDMOREPARAMS(user->getNickname(), "PONG"));
        return;
    }
    user->SendMsg(RPL_PING(user->getMessage(), args[0]));
}
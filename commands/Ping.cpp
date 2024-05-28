#include "Command.hpp"

Kick(Server *srv) : Command(srv) {}

Kick::~Kick() {}

void Kick::execute(User *user, std::vector<std::string> args)
{
    if (args.empty())
    {
        user->ReplyMsg(ERR_NEEDMOREPARAMS(user->getNickname(), "PING"));
        return;
    }
    user->SendMsg(RPL_PING(user->getMessage(), args[0]));
}
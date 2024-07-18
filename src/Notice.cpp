#include "Command.hpp"

Notice::Notice(Server& srv) : Command(srv) {}

Notice::~Notice() {}

void    Notice::execute(User& client, std::vector<std::string> args)
{
    (void)args;
    (void)client;
}
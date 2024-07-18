#include "../includes/Command.hpp"

Who::Who(Server& srv) : Command(srv) {}

Who::~Who() {}

void    Who::execute(User& client, std::vector<std::string> args)
{
    (void)args;
    (void)client;
}
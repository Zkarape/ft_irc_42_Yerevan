
#include "Command.hpp"

Nick::Nick(Server& srv, bool auth) : Command(srv, auth) {}

Nick::~Nick() {}

// syntax: NICK <nickname>

bool nickIsCorrect(std::string buffer)
{
    std::string notAllowed = " ,*?!@$:#.";
    std::size_t pos = buffer.find_first_of(notAllowed);
    if (pos != std::string::npos)
    {
        return false;
    }
    return true;
}

void    Nick::execute(User& client, std::vector<std::string> args)
{
    if (client.getPassword().empty())
    {
        client.reply(ERR_NOTREGISTERED(client.getNickname()));
        return ;
    }
    if (args.empty() || args[0].empty()) 
    {
        client.reply(ERR_NONICKNAMEGIVEN(client.getNickname()));
        return;
    }

    std::string nickname = args[0];

    if (!nickIsCorrect(nickname))
    {
        client.reply(ERR_ERRONEUSNICKNAME(client.getNickname(), nickname));
        return ;
    }

    if (_srv.getClient(nickname))
    {
        client.reply(ERR_NICKNAMEINUSE(client.getNickname(), nickname));
        return;
    }

    client.setNickname(nickname);
    client.checkForRegistered();
}
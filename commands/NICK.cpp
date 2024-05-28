#include "Command.hpp"

Nick::Nick(Server *server) : Command(server)
{}

Nick::~Nick()
{}

bool Nick::is_valid(std::string nick)
{
	for (size_t i = 0; i < nick.size(); i++)
	{
		char c = nick[i];
		if (c != '[' && c != ']' && c != '\\' && c != '`' && c != '_'
			&& c != '^' && c != '{' && c != '}' && c != '|' &&
			!std::isalpha(c))
				return (false);
	}
	return (true);
}


void Nick::execute(User& user, Args args)
{
	if (args.size() != 1)
		user.reply(ERR_NONICKNAMEGIVEN(user.get_nickname()));
	else if (serv->is_client(args[0]))
		user.reply(ERR_NICKNAMEINUSE(user.get_nickname(), args[0]));
	else if (!is_valid(args[0]))
		user.reply(ERR_ERRONEUSNICKNAME(user.get_nickname(), args[0]));
	else if (user.has_registered())
	{
		std::string old_nick = user.get_nickname();
		user.set_nickname(args[0]);
		///TODO: Zhenya server notify_clients
		serv->notify_clients(":" + old_nick + " NICK " + args[0]);
	}
	else
	{
		user.set_nickname(args[0]);
		user.welcome();
	}
}
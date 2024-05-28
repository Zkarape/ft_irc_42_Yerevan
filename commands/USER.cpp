#include "Command.hpp"

User_cmd::User_cmd(Server *server) : Command(server)
{}

User_cmd::~User_cmd()
{}

void User_cmd::execute(User& user, Args args)
{
	if (args.size() < 4)
		user.reply(ERR_NEEDMOREPARAMS(user.get_nickname(), "USER"));
	else if (user.has_registered())
		user.reply(ERR_ALREADYREGISTERED(user.get_nickname()));
	else
	{
		user.set_username(args[0]);
		user.set_hostname(args[2]);
		user.welcome();
	}
}
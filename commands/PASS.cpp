#include "Command.hpp"
#include "User.hpp"

Pass::Pass(Server *server) : Command(server) 
{}

Pass::~Pass() 
{}

void Pass::execute(User& user, Args args)
{
	if (user.has_passed())
	{
		user.reply(ERR_ALREADYREGISTERED(user.get_nickname()));
		return ;
	}
	if (args.size() != 1)
	{
		user.reply(ERR_NEEDMOREPARAMS(user.get_nickname(), "PASS"));
		return ;
	}
	if (args[0][0] == ':')
		args[0].erase(0, 1);
	///TODO: Zhenya server get_password
	if (args[0] != serv->get_password())
		user.reply(ERR_PASSWDMISMATCH(user.get_nickname()));
	else
	{
		user.set_passed();
		user.welcome();
	}
}

#include "Command.hpp"

User::User(Server *server) : Command(server)
{}

User::~User()
{}

void User::execute(Client& client, Args args)
{
	if (args.size() < 4)
		client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "USER"));
	else if (client.has_registered())
		client.reply(ERR_ALREADYREGISTERED(client.get_nickname()));
	else
	{
		client.set_username(args[0]);
		client.set_hostname(args[2]);
		client.welcome();
	}
}
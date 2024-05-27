#pragma once

#include <vector>
#include <string>
#include <cctype>
#include <iostream>

#include "ErrNum.hpp"
#include "Client.hpp"
#include "server.hpp"

// #include "Channel.hpp"

class Server;
class Client;
class Channel;

typedef std::vector<std::string>      Args;
// typedef std::string                   Name;

class Command
{
	public:
		Command(Server *server);
		virtual ~Command();
		virtual void execute(Client&, Args) = 0;
	protected:
		Server *serv;
};

class Pass : public Command 
{
	public:
		Pass(Server *server);
		~Pass();
		void execute(Client&, Args);
};

class Nick : public Command 
{
	public:
		Nick(Server *server);
		~Nick();
		bool is_valid(std::string nick);
		void execute(Client&, Args);
};

class User : public Command 
{
	public:
		User(Server *server);
		~User();
		void execute(Client&, Args);
};

// class Join : public Command 
// {
// 	public:
// 		Join(Server *server);
// 		~Join();
// 		void execute(Client&, Args);
// };

// class Kick : public Command 
// {
// 	public:
// 		Kick(Server *server);
// 		~Kick();
// 		void execute(Client&, Args);
// };

// class Privmsg : public Command 
// {
// 	public:
// 		Privmsg(Server *server);
// 		~Privmsg();
// 		void execute(Client&, Args);
// 	};

// class Notice : public Command 
// {
// 	public:
// 		Notice(Server *server);
// 		~Notice();
// 		void execute(Client&, Args);
// };

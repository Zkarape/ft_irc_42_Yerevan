#pragma once

#include "User.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <string>

class Server;
class User;

class Command
{
    protected:
        Command();
        Command(const Command& srv);

        Server* _srv;
        bool _auth;
    public:
        Command(Server* srv, bool auth = true);
        virtual ~Command();

        bool auth_required() const;

        virtual void execute(User& user, std::vector<std::string> args) = 0;
};

class Privmsg : public Command
{
    public:
       Privmsg(Server *srv);
       ~Privmsg();

       void    execute(User& user, std::vector<std::string> args);

};

class Notice : public Command
{
    public:
        Notice(Server *srv);
        ~Notice();

        void    execute(User& user, std::vector<std::string> args);

};

class Join : public Command
{
    public:
        Join(Server *srv);
        ~Join ();

         void    execute(User& user, std::vector<std::string> args);
};

class Invite : public Command
{
    public:
        Invite(Server *srv);
        ~Invite ();

        void    execute(User& user, std::vector<std::string> args);
};

class Kick : public Command
{
    public:
        Kick(Server *srv);
        ~Kick();

        void    execute(User& user, std::vector<std::string> args);
};

class Topic : public Command
{
    public:
        Topic(Server *srv);
        ~Topic();

        void    execute(User& user, std::vector<std::string> args);
};

class Nick : public Command
{
    public:
        Nick(Server *srv);
        ~Nick();

        void    execute(User& user, std::vector<std::string> args);
};

class Pass : public Command
{
    public:
        Pass(Server *srv);
        ~Pass();

        void    execute(User& user, std::vector<std::string> args);
};

class User_cmd : public Command
{
    public:
        User_cmd(Server *srv);
        ~User_cmd();

        void    execute(User& user, std::vector<std::string> args);
};

class Mode : public Command
{
    public:
        Mode(Server *srv);
        ~Mode();

        void    execute(User& user, std::vector<std::string> args);
};

class Ping : public Command
{
    public:
        Ping(Server *srv);
        ~Ping();

        void    execute(User& user, std::vector<std::string> args);
};

class Pong : public Command
{
    public:
        Pong(Server *srv);
        ~Pong();

        void    execute(User& user, std::vector<std::string> args);
};
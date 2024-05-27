#pragma once

#include "Client.hpp"
#include "Replies.hpp"
#include "Server.hpp"


#include <string>

class Command
{
    protected:
        Command();
        Command(const Command& srv);

        Server* _srv;
        bool _auth;
    public:
        explicit Command(Server* srv, bool auth = true);
        virtual ~Command();

        bool auth_required() const;

        virtual void execute(Client* client, std::vector<std::string> args) = 0;
};

class Privmsg : public Command
{
    public:
       Privmsg(Server *srv);
       ~Privmsg();

       void    execute(Client* client, std::vector<std::string> args);

};

class Notice : public Command
{
    public:
        Notice(Server *srv);
        ~Notice();

        void    execute(Client* client, std::vector<std::string> args);

};

class Join : public Command
{
    public:
        Join(Server *srv);
        ~Join ();

         void    execute(Client* client, std::vector<std::string> args);
};

class Invite : public Command
{
    public:
        Invite(Server *srv);
        ~Invite ();

        void    execute(Client* client, std::vector<std::string> args);
};

class Kick  : public Command
{
    public:
        Kick(Server *srv);
        ~Kick();

        void    execute(Client* client, std::vector<std::string> args);
};

class Topic : public Command
{
    public:
        Topic(Server *srv);
        ~Topic();

         void    execute(Client* client, std::vector<std::string> args);
};
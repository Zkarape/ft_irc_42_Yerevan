#pragma once

#include "User.hpp"
#include <vector>
#include <string>
#include "commands/Replies.hpp"

class User;

class Channel
{
    private:
        std::string _name;
        std::string _pswd;
        std::string _topic;
        std::string _mode;

        std::vector<User *> _users;
        std::vector<User *> _operators;
        std::vector<User *> _invitees;

        bool    _invite_only;
        bool    _topic_operators;
        int     _limit;

        User *_owner;

    public:
        Channel(std::string &name, std::string &pswd, User *owner);
        ~Channel();

        std::string getName();
        std::string getPswd();
        std::string getTopic();
        std::string getMode();
        
        std::vector<User *> getUsers();

        User        *getUser(const std::string &nickname);
        User        *getOwner();
        int         getLimit();

        void        setName(const std::string &name);   //
        void        setPswd(const std::string &pswd);
        void        setTopic(const std::string &topic);
        void        setMode(const std::string &mode);
        void        setOwner(User *owner);
        void        setInvite(User *user);
        void        setOperator();

        void        unsetOperator();
        void        unsetInvite(User *user);

        void        applyMode();

        void        join(User *user);
        void        part(User *user);
        void        kick(User *user, const std::string &msg);

        void        sendMsgToChannel(User *user, const std::string &msg);

        bool        isOwner(User *user);
        bool        isOperator(User *user);
        bool        isExist(User *user);
        bool        isInvite(User *user);
        bool        isInviteOnly();
        bool        isTopicOperators();

};

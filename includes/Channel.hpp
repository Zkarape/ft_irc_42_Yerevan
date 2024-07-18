#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "replies.hpp"
#include <set>
#include <list>

class User;

class Channel
{
    public:
        Channel(const std::string& name, const std::string& pass = "", User *client = NULL);
        ~Channel();

        void deleteClient(User &client);
        void joinClient(User &client);
        bool isInChannel(User& client);
        void sending(User* C, const std::string& msg);
        bool channelIsFull(void);
        bool emptyChannel(void);
        size_t get_limit() const;
        size_t get_size()const;
        std::string get_pass() const;
        void set_pass(const std::string& pass);
        std::string getName(void) const;
        void broadcast(const std::string& message);
        void broadcast(const std::string& message, User* exclude);
        void print() const;
        void setChannelLimit(int limit);
        int getChannelLimit(void);
        bool changeClientMode(User& client, int type);
        void nameReply(User &client);
        bool emptyClients(void);
        const User &getAdmin() const;
        void setInviteOnly(bool mode);
        bool isInviteOnly(void);
        bool isOperator(User& client) const;
        bool isEmpty() const;
        void setTopicMode(bool mode);
        bool isTopicModeOn() const;
        User *getClienByItstNick(const std::string& nickname);
        bool isAdmin(const User& client) const;
        void sendMsg(User &client, const std::string &msg, const std::string& cmd);
        std::string getTopic(void) const;
        void setTopic(const std::string& topic);

    private:
        std::map<int, User *> _clients;
        std::set<User *> _operators;
        std::list<User *> _primary;
        User *_admin;
        std::list<User *> _listOperator;
        size_t _limit;
        bool _inviteOnly;
        bool _topicMode;
        
    public:
        std::string _name;
        std::string _pass;
        std::string _topic;
};


#endif
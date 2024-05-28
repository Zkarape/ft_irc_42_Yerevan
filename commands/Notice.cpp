#include "Command.hpp"

Notice::Notice(Server *srv) : Command(srv) {}

Notice::~Notice() {}

void Notice::execute(User *user, std::vector<std::string> args)
{
    if (args.size() < 2 || args[0].empty() || args[1].empty())
    {
        user->reply(ERR_NEEDMOREPARAMS(user->get_nickname(), "Notice"));
        return;
    }

    // std::string target = args[0];
    // std::string message;

    // std::vector<std::string>::iterator it = args.begin() + 1;
    // std::vector<std::string>::iterator end = args.end();

    // while (it != end)
    // {
    //     message.append(*it + " ");
    //     it++;
    // }

    // if (message.at(0) == ':')
    //     message = message.substr(1);

    // if (target.at(0) == '#')
    // {
    //     Channel* channel = user->get_channel();

    //      if (!channel)
    //     {
    //         user->reply(ERR_NOSUCHNICK(user->get_nickname(), target));
    //         return;
    //     }

    //     if (!channel->is_member())
    //     {
    //         std::vector<std::string> nicknames = channel->get_nicknames();

    //         std::vector<std::string>::iterator it = nicknames.begin();
    //         std::vector<std::string>::iterator end = nicknames.end();

    //          while (it != end)
    //         {
    //             if (*it == user->get_nickname())
    //                 break;

    //             it++;
    //         }

    //         if (it == end)
    //         {
    //             user->reply(ERR_CANNOTSENDTOCHAN(user->get_nickname(), target));
    //             return;
    //         }
    //     }

    //     channel->broadcast(RPL_NOTICE(user->get_prefix(), target, message), user);
    //     return;
    // }

    // User  *dest = _srv->get_client(target);
    // if (!dest)
    // {
    //     user->reply(ERR_NOSUCHNICK(user->get_nickname(), target));
    // 	return;
    // }

    // dest->write(RPL_NOTICE(user->get_prefix(), target, message));
    std::string target = obj[0];
    std::string message = obj[1];

    if (message[0] == ':')
        message.erase(0, 1);
    for (size_t i = 2; i < obj.size(); i++)
        message += " " + obj[i];

    if (target[0] == '#' || target[0] == '&')
    {
        Channel *channel = _Server->getChannel(target);
        if (channel == NULL)
        {
            user->SendMsg(ERR_NOSUCHCHANNEL(user->getNickname(), target));
            return;
        }
        if (!channel->isExist(user))
        {
            user->SendMsg(ERR_NOTONCHANNEL(user->getNickname(), target));
            return;
        }
        channel->sendMsgToChannel(user, RPL_NOTICE(user->getNickname(), target, message));
        user->SendMsg(RPL_NOTICE(user->getNickname(), target, message));
        return;
    }
    else
    {
        User *targetUser = _Server->getUser(target);
        if (targetUser == NULL)
        {
            user->SendMsg(ERR_NOSUCHNICK(user->getNickname(), target));
            return;
        }

        targetUser->SendMsg(RPL_NOTICE(user->getNickname(), target, message));
        user->SendMsg(RPL_NOTICE(user->getNickname(), target, message));
    }
}

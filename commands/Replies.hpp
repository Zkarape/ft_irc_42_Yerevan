#pragma once

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <time.h>

//Error replies

#define ERR_NOSUCHNICK(source, command)         "401 " + source + " " + command + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL(source, command)      "403 " + source + " " + command + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(source, command)   "404 " + source + " " + command + " :Cannot send to channel"
#define ERR_NORECIPIENT(source, command)        "411 " + source + " :No recipient given (" + command + ")"
#define ERR_NOTEXTTOSEND(source)                "412 " + source + " :No text to send"
#define ERR_UNKNOWNCOMMAND(source, command)     "421 " + source + " " + command + " :Unknown command"
#define ERR_NONICKNAMEGIVEN(source)             "431 " + source + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(source, command)   "432 " + source + " " + command + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(source, command)      "433 " + source + " " + command + " :Nickname is already in use"
#define ERR_USERNOTINCHANNEL(source, c1, c2)   "441 " + source + " " + c1 + " " + c2 + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(source, command)       "442 " + source + " " + command + " :You're not on that channel"
#define ERR_USERONCHANNEL(source, c1, c2)      "443 " + source + " " + c1 + " " + c2 + " :is already on channel"
#define ERR_NOTREGISTERED(source)               "451 " + source + " :You have not registered"
#define ERR_NEEDMOREPARAMS(source, command)     "461 " + source + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(source)           "462 " + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)              "464 " + source + " :Password incorrect"
#define ERR_CHANNELISFULL(source, command)      "471 " + source + " " + command + " :Cannot join channel, channel is full (+l)"
#define ERR_INVITEONLYCHAN(source, command)     "473 " + source + " " + command + " :Cannot join channel, you must be invited (+i)"
#define ERR_BADCHANNELKEY(source, command)      "475 " + source + " " + command + " :Cannot join channel (+k)"
#define ERR_BADCHANMASK(source, command)        "476 " + source + " " + command + " :Bad Channel Mask"
#define ERR_CHANOPRIVSNEEDED(source, command)   "482 " + source + " " + command + " :You're not channel operator"

//Command replies

#define RPL_WELCOME(source)                     "001 " + source + " :Welcome to the ft_irc Network"
#define RPL_ENDOFWHO(name, source)              "315 " + name + " " + source + " :End of /WHO list"
#define RPL_NOTOPIC(source, channel)             "331 " + source + " " + channel + " :No topic is set"
#define RPL_TOPIC(source, channel, topic)       "332 " + source + " " + channel + " :" + topic
#define RPL_INVITING(name, nickname, channel)            "341 " + name + " " + nickname + " " + channel
#define RPL_WHOREPLY(name, channel, user, hostname, nickname, file, reply)"352 " + name + " " + channel + " " + user + " " + hostname + " ft_irc " + nickname + " " + file + " :1 " + reply
#define RPL_NAMREPLY(name, channel, prefix, nickname)    "353 " + name + " = " + channel + " :" + prefix + nickname
#define RPL_ENDOFNAMES(name, channel)                "366 " + name +  " " + channel + " :End of /NAMES list"
#define RPL_CHANNELMODEIS(name, channel, mode)       "324 " + name + " " + channel + " " + mode
#define RPL_MSG(prefix, command, target, message)   ":" + prefix + " " + command + " " + target + " :" + message
#define RPL_KICK(prefix, channel, target, message)   ":" + prefix + " KICK " + channel + " " + target + " :" + message
#define RPL_PING(prefix, message)                   ":" + prefix + " PONG :" + message
#define RPL_QUIT(prefix, message)                   ":" + prefix + " QUIT :Quit: " + message
#define RPL_JOIN(prefix, target)                    ":" + prefix + " JOIN " + target
#define RPL_PART(prefix, target)                    ":" + prefix + " PART " + target
#define RPL_MODE(prefix, channel, target)           ":" + prefix + " MODE " + channel + " " + target
#define RPL_INVITE(prefix, target, channel)         ":" + prefix + " INVITE " + target + " " + channel
#define RPL_NOTICE(prefix, target, message)         ":" + prefix + " NOTICE " + target + " :" + message

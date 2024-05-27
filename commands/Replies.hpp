#pragma once

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <time.h>

//Error replies

#define ERR_NEEDMOREPARAMS(source, command)         "461 " + source + " " + command + " :Not enough parameters"
#define ERR_NOSUCHNICK(source, nickname)            "401 " + source + " " + nickname + " :No such nickname"
#define ERR_CANNOTSENDTOCHAN(source, channel)       "404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_TOOMANYCHANNELS(source, channel)        "405 " + source + " " + channel + " :Too many channels joined"
#define ERR_CHANNELISFULL(source, channel)          "471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_BADCHANNELKEY(source, channel)          "475 " + source + " " + channel + " :Cannot join channel (+k)"

//Command replies

#define RPL_PRIVMSG(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)             ":" + source + " NOTICE " + target + " :" + message
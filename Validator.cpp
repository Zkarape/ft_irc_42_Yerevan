#include <iostream>

///TODO: im not sure, which library is ok and which one is not
#include <vector>
#include <cctype>

#include <vector>
#include <string>

#include "Command.hpp"

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    // First token
    char *token = strtok(const_cast<char*>(str.c_str()), const_cast<char*>(delim.c_str()));
    std::vector<std::string> substr;
   
    while (token != NULL)
    {
        substr.push_back(token);
        token = strtok(NULL, const_cast<char*>(delim.c_str()));
    }
    return (substr);
}


// std::vector<std::string> split(const std::string &str, const std::string &delimiter)
// {
// 	std::vector<std::string> result;
// 	std::string::size_type start = 0;
// 	std::string::size_type end = 0;

// 	while ((end = str.find(delimiter, start)) != std::string::npos)
// 	{
// 		std::string token = str.substr(start, end - start);
// 		if (!token.empty())
// 			result.push_back(token);
// 		start = end + delimiter.length();
// 	}

// 	// Add the last token
// 	std::string token = str.substr(start);
// 	if (!token.empty())
// 		result.push_back(token);

// 	return (result);
// }

void	prefix_p(std::vector<std::string> & arr, int & tkn, Server & server, Client & client)
{
	if (arr[tkn][0] == ':') 
	{
		///TODO: prefix validation
		if ("servername/host stuff")
		{
			/* code */		
		}
		else if ("nick stuff")
		{
			/* code */
		}
		tkn++;
	}
}

void	command_p(std::vector<std::string> & arr, int & tkn, Server & server, Client & client)	// command validation
{
	std::string nm;

	nm = arr[tkn];
	if (isalpha(arr[tkn][0]))
	{
		// validating comand names
		if (!(nm == "PRIVMSG" || nm == "NOTICE"
        || nm == "PASS" || nm == "USER"
        || nm == "NICK" || nm == "JOIN"
        || nm == "KICK"))
			throw(std::runtime_error("Invalid input"));
	}
	else if (isdigit(arr[tkn][0]))
	{
		for (size_t i = 1; i < 3; i++)
			if(isdigit(arr[tkn][i]))
				throw(std::runtime_error("Invalid input"));
		if (nm == "001" || nm == "315" || nm == "324" || nm == "331" ||
			nm == "332" || nm == "341" || nm == "352" || nm == "353" ||
			nm == "366" || nm == "401" || nm == "403" || nm == "404" ||
			nm == "411" || nm == "412" || nm == "421" || nm == "431" ||
			nm == "432" || nm == "433" || nm == "441" || nm == "442" ||
			nm == "443" || nm == "451" || nm == "461" || nm == "462" ||
			nm == "464" || nm == "467" || nm == "471" || nm == "472" ||
			nm == "473" || nm == "475" || nm == "477" || nm == "476" ||
			nm == "482")
		{
			///TODO: ERROR stuff
		}
		else
			throw(std::runtime_error("Invalid input")); // et tvov error comman chka tip@
	}
	else
		throw(std::runtime_error("Invalid input"));
	tkn++;
	return (name);
}

void	param_p(std::vector<std::string> & arr, int & tkn, Server & server, Client & client)	// params validation
{
	if (arr[tkn][0] == ':') // trailing
	{
		for (size_t i = 0; i < arr[tkn].length(); i++)
			if (arr[tkn][i] == 0 || arr[tkn][i] == '\r' || arr[tkn][i] == '\n')
				throw(std::runtime_error("Invalid input"));
		///TODO: validating trailing names
	}
	else // middle
	{
		for (size_t i = 0; i < arr[tkn].length(); i++)
			if (arr[tkn][i] == 0 || arr[tkn][i] == '\r' || arr[tkn][i] == '\n')
				throw(std::runtime_error("Invalid input"));
		///TODO: validating middle names
	}
	tkn++;

	if (!(arr[tkn][0] == '\r' && arr[tkn][1] == '\n'))
		param_p(arr, tkn);
}

void executor(std::string name, std::vector<std::string> & arr, Server & server, Client & client)
{
	if (name == "PRIVMSG")
	{
		Privmsg cmd(server);
		cmd.execute(client, arr);
	}
	else if (name == "JOIN")
	{
		Join cmd(server);
		cmd.execute(client, arr);
	}
	else if (name == "NOTICE")			
	{
		Notice cmd(server);
		cmd.execute(client, arr);
	}
	else if (name == "PASS")
	{
		Pass cmd(server);
		cmd.execute(client, arr);
	}
	else if (name == "USER")
	{
		User cmd(server);
		cmd.execute(client, arr);
	}
	else if (name == "NICK")
	{
		Nick cmd(server);
		cmd.execute(client, arr);
	}
}

void validator(std::string message, Server & server, Client & client)
{
	try
	{
		std::vector<std::string> arr = split(message, " ");
		int tkn = 0;
		std::string name;
			// int commId;

		// CR LF validation (in the end)
		if (!(arr[arr.length()][0] == '\r' && arr[arr.length()][1] == '\n'))
			throw(std::runtime_error("Invalid input"));

		prefix_p(arr, tkn, server, client);
		name = command_p(arr, tkn, server, client);

		if (!(arr[tkn][0] == '\r' && arr[tkn][1] == '\n')) // kara param chlini \r\n-ic araj
			param_p(arr, tkn, server, client);

		///TODO:: check arr part
		executor(arr, name, server, client);
		// return ;
	}
	catch (const std::exception& e)
		std::cerr << e.what() << '\n';
}

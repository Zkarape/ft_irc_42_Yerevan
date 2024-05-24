#include <iostream>

///TODO: vrode chi kareli avel library 
#include <vector>
#include <cctype>

#include <iostream>
#include <vector>
#include <string>

///TODO: make it smort (lexer-esque)
std::vector<std::string> split(const std::string &str, const std::string &delimiter)
{
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end = 0;

	while ((end = str.find(delimiter, start)) != std::string::npos)
	{
		std::string token = str.substr(start, end - start);
		if (!token.empty())
			result.push_back(token);
		start = end + delimiter.length();
	}

	// Add the last token
	std::string token = str.substr(start);
	if (!token.empty())
		result.push_back(token);

	return (result);
}

void	prefix_p(std::vector<std::string> & arr, int & tkn)	// prefix validation
{
	if (arr[tkn][0] == ':') 
	{
		///TODO: prefix validation in depth
		if ("servername/host")
		{
		
		}
		else if ("nick")
		{
			/* code */
		}
		tkn++;
	}
}

void	command_p(std::vector<std::string> & arr, int & tkn)	// command validation
{
	if (isalpha(arr[tkn][0]))
	{
		// validating comand names
		if (arr[tkn] == "PRIVMSG")
		{
			///TODO: add the functions when ready
			// we call the PRIVMSG function
		}
		else if (arr[tkn] == "JOIN")
		{
			// etc
		}
		else if (arr[tkn] == "NOTICE")			
		{

		}
		else
			throw(std::runtime_error("Invalid input"));
	}
	else if (isdigit(arr[tkn][0]))
	{
		for (size_t i = 1; i < 3; i++)
			if(isdigit(arr[tkn][i]))
				throw(std::runtime_error("Invalid input"));
		if (arr[tkn] == "number stuff")
		{
			///TODO: ERROR BS | kanchi tvin hamapatasxan error
		}
		else
			throw(std::runtime_error("Invalid input")); // et tvov error comman chka tip@
	}
	else
		throw(std::runtime_error("Invalid input"));
	tkn++;
}

void	param_p(std::vector<std::string> & arr, int & tkn)	// params validation
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

void validator(std::string message)
{
	try
	{
		std::vector<std::string> arr = split(message, " ");
		int tkn = 0;
			// int commId;

		// CR LF validation (in the end)
		if (!(arr[arr.length()][0] == '\r' && arr[arr.length()][1] == '\n'))
			throw(std::runtime_error("Invalid input"));

		prefix_p(arr, tkn);
		command_p(arr, tkn);

		if (!(arr[tkn][0] == '\r' && arr[tkn][1] == '\n')) // kara param chlini \r\n-ic araj
			param_p(arr, tkn);

		// return ;
	}
	catch (const std::exception& e)
		std::cerr << e.what() << '\n';
}

#include <iostream>

///TODO: vrode chi kareli avel library 
#include <vector>
#include <cctype>

#include <iostream>
#include <vector>
#include <string>

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

	return result;
}

void validator(std::string message)
{
	try
	{
		int tkn = 0;
		std::vector<std::string> arr = split(message, " ");
		
		// prefix validation
		if (arr[tkn][0] == ':') 
		{
			// prefix validation in depth
			tkn++;
		}

		// command validation
		if (isalpha(arr[tkn][0])) // prefixi depqum arten araja gnum esli cho u 0-n urisha
		{
			if (/* condition */)
			{
				// validating comand names
			}
			else
				throw(std::runtime_error("Invalid input"));
		}
		else if (isdigit(arr[tkn][0]))
		{
			for (size_t i = 1; i < 3; i++)
			{
				if(isdigit(arr[tkn][i]))
					throw(std::runtime_error("Invalid input"));
			}
			if (/* condition */)
			{
				// validating comand names in 3-digit format
			}
			else
				throw(std::runtime_error("Invalid input"));
		}
		else
		{
			throw(std::runtime_error("Invalid input"));
		}

		tkn++;
		///TODO: KARA PARAMCHLINI \r\n-ic araj
		// params validation
		if (arr[tkn][0] == ':') // trailing
		{
			for (size_t i = 0; i < arr[tkn].length(); i++)
				if (arr[tkn][i] == 0 || arr[tkn][i] == '\r' || arr[tkn][i] == '\n')
					throw(std::runtime_error("Invalid input"));
			// validating trailing names
		}
		else if (/* condition */) // middle
		{
			for (size_t i = 0; i < arr[tkn].length(); i++)
				if (arr[tkn][i] == 0 || arr[tkn][i] == '\r' || arr[tkn][i] == '\n')
					throw(std::runtime_error("Invalid input"));
			// validating middle names

			// params again (recursive???)
		}

		tkn++;
		// CR LF validation
		if (!(arr[tkn][0] == '\r' && arr[tkn][0] == '\n)'))
			throw(std::runtime_error("Invalid input"));
		
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		// return ;
	}
}

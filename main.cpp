#include "includes/Server.hpp"
//#include "includes/Client.hpp"
#include <iostream>

int	portChecker(std::string string) 
{
	for (size_t i = 0; i < string.length(); i++) 
	{
		if (!isdigit(string[i])) 
		{
			std::cerr << "Invalid port" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	int port = atoi(string.c_str());
	if (port < 0 || port > 65535) //< o <= 0???
	{
		std::cerr << "Invalid port" << std::endl;
		exit(EXIT_FAILURE);
	}
	return (port);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./irc <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}
	Server server(portChecker(argv[1]), argv[2]);
	try
	{
		signal(SIGINT, Server::checkSignal);
		signal(SIGQUIT, Server::checkSignal);
		server.InitializeServer();
		server.Run();
	}
	catch(const std::exception& e)
	{
		//server.fdClose();
		std::cerr << e.what() << '\n';
	}
}

#include "includes/Server.hpp"
//#include "includes/Client.hpp"
#include <iostream>

in_port_t	portChecker(std::string string) 
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
	if (port < 0 || port > 65535) 
	{
		std::cerr << "Invalid port" << std::endl;
		exit(EXIT_FAILURE);
	}
	return (port);
}

bool ServerisRunning;

int main(int argc, char **argv)
{
	ServerisRunning = true;

	if (argc != 3)
	{
		std::cerr << "Usage: ./irc <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		Server server(portChecker(argv[1]), std::string(argv[2]));
		server.InitializeServer();
		server.Run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

#include "includes/Server.hpp"
//#include "includes/Client.hpp"
#include <iostream>


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./irc <port> <password>" << std::endl;
		return (1);
	}
	Server server(atoi(argv[1]), argv[2]);
	try
	{
		server.Initialize();
		server.Run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

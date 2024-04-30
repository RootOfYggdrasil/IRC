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
	server.Start();
	while (true)
	{
		std::cout << server.getPort() << std::endl;
		std::cout << server.getSocket() << std::endl;
		read(0, NULL, 1);
	}
}
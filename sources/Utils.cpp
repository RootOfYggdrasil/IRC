#include "Utils.hpp"


std::string	toUpperString(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

std::string	toLowerString(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

void checkSignal(int signal)
{
	std::cout << "Signal received" << std::endl;
	if (signal == SIGINT)
		std::cout << std::endl << "\033[F" << "SIGINT signale recieved, servere closed."<< std::endl;
	exit(1);
}

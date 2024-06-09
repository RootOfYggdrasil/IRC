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
	ServerisRunning = false;
	if (signal == SIGINT || signal == SIGQUIT)
		std::cout << std::endl << "\033[F" << "SIGINT signal recieved, server closed."<< std::endl;
}

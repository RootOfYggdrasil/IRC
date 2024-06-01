#include <string>
#include <algorithm>

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

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <algorithm>
# include <signal.h>
# include <iostream>
# include "Server.hpp"

std::string	toUpperString(std::string str);
std::string	toLowerString(std::string str);
void checkSignal(int);

#endif

#pragma once

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include <vector>
# include <iostream>
# include <string>
# include <sstream>

class Server;
class Client;
class Channel;

class Command {
	private:
		Command(void);
		~Command(void);
	public:
		static void	join(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void	privmsg(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void	invite(Server &server, Client &client, std::string &command);
		static void	bot(Server &s, Client &client, std::vector<std::string> &vArguments);
		static void	bot(Server &s, Client &client, std::string &command);
		static void nick(Server &server, Client &client, std::vector<std::string> &vArguments);
}; 

#endif

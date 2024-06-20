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
		static void	bot(Server &s, Client &client, std::vector<std::string> &vArguments);
		static void nick(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void kick(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void quit(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void topic(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void mode(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void inv(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void user(Server &server, Client &client, std::vector<std::string> &vArguments);
		static void password(Server &server, Client &client, std::vector<std::string> &v);
}; 

#endif


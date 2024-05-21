#pragma once

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Server;
class Client;
class Channel;

class Command {
	private:
		Command(void);
		~Command(void);
	public:
		static void	join(Server &server, Client &client, std::string &command);
		static void	ping(Server &server, Client &client, std::string &command);
		static void	pass(Server &server, Client &client, std::string &command);
		static void	nick(Server &server, Client &client, std::string &command);
		static void	user(Server &server, Client &client, std::string &command);
		static void	quit(Server &server, Client &client, std::string &command);
		static void	privmsg(Server &server, Client &client, std::string &command);
		static void	notice(Server &server, Client &client, std::string &command);
		static void	invite(Server &server, Client &client, std::string &command);
}; 

#endif

#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Channel.hpp"
# include "Command.hpp"
# include <vector>

class Server;
class Channel;

class Client {
	private:
		int				_clientFd;
		std::string		_nickname;
		std::string		_user;
		std::string		_buffer;
		bool			_isLogged;
		bool			_hasPw;

		std::map <std::string, Channel *>	_connectedOnChannel;

	public:
		Client(void);
		Client(int fd);
		Client &operator=(const Client &op);
		~Client(void);

		int getFd() const;
		bool getIsLogged() const;
		bool getPw() const;
		const std::string &getUsername() const;
		std::string getNickname() const;
		std::string getBuffer() const;
		Channel *getChannel(const std::string &channelName) const;
		
		void setIsLogged(bool logged);
		void setNikcname(const std::string nickname);
		void setBuffer(std::string buffer);
		void setPw(bool pw);
		void setUsername(std::string &user);

		void deleteChannelJoined(Channel *ch);
//		void deleteClFromCh(Server &server);

		void addChannel(Channel *channel);
}; 

std::ostream &operator<<(std::ostream &o, const Client &cl);
std::ostream &operator<<(std::ostream &o, const Client *cl);

#endif

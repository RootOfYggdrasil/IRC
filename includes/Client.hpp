#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Channel.hpp"
# include "Command.hpp"
# include <vector>

class Server;

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
		Client(int fd);
		Client &operator=(const Client &op);
		~Client(void);

		const int &getFd() const;
		bool getIsLogged() const;
		bool getPw() const;
		std::string getNickname() const;
		std::string getBuffer() const;
		
		void setIsLogged(bool logged);
		void setBuffer(std::string &buffer);
		void setPw(bool pw);

		void deleteClfromChannel(Server &server);


}; 

#endif

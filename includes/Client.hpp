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

		std::map <std::string, Channel *>	_connectedOnChannel;

	public:
		Client(void);
		Client(int fd);
		Client(int fd);
		Client &operator=(const Client &op);
		~Client(void);

		const int &getFd() const;
<<<<<<< HEAD
		bool getIsLogged() const;
=======
		void getIsLogged() const;
>>>>>>> 7bc606e8ebbbc3da34d85132fcbfb9718efdb1f4
		std::string getBuffer() const;
		
		void setIsLogged(bool logged);
		void setBuffer(std::string &buffer);

		void deleteClfromChannel(Server &server);
<<<<<<< HEAD
=======

>>>>>>> 7bc606e8ebbbc3da34d85132fcbfb9718efdb1f4

}; 

#endif

#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Channel.hpp"
# include "Command.hpp"
# include <vector>


class Client {
	private:
		int				_clientFd;
		std::string		_nickname;
		std::string		_user;
		bool			_isLogged;

		std::map <std::string, Channel *>	_joinedChannel;

	public:
		Client(void);
		Client(int fd);
		Client &operator=(const Client &op);
		~Client(void);

		const int &getFd() const;

}; 

#endif

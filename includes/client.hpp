#pragma once

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client {
	private:


	public:
		Client(void);
		Client(const Client &copy);
		Client &operator=(const Client &op);
		~Client(void);

}; 

#endif

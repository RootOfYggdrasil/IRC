#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Command.hpp"

class Channel {
	private:


	public:
		Channel(void);
		Channel(const Channel &copy);
		Channel &operator=(const Channel &op);
		~Channel(void);

}; 

#endif

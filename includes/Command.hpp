#pragma once

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Command {
	private:


	public:
		Command(void);
		Command(const Command &copy);
		Command &operator=(const Command &op);
		~Command(void);

}; 

#endif

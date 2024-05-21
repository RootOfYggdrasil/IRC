#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Command.hpp"

class Channel {
	private:
		std::string	_name;
		std::string	_password;
		std::string	_topic;
		std::string	_creationTime;

		bool		_inviteOnly;
		bool		_topicRestrict;

		int			_clientsMax;

		//clients
		//operatori

	public:
		Channel();
		Channel &operator=(const Channel &op);
		~Channel();

		bool	getInviteOnly();
		bool	getTopicRestrict();
		int		getClientsMax();
		void	setTopic(std::string topic);
		void	setTopicRestrict(bool topicRestrict);
		void	setInviteOnly(bool inviteOnly);
		void	setClientsMax(int clientsMax);

		

}; 

#endif

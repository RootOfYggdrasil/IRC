#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Command.hpp"

class Client;

class Channel {
	private:
		std::string	_name;
		std::string	_password;
		std::string	_topic;
		std::string	_creationTime;

		bool		_inviteOnly;
		bool		_topicRestrict;

		int			_clientsMax;
		std::map<std::string, Client*>	_clients;
		//clients
		//operatori

	public:
		Channel();
		Channel(std::string name);
		Channel(std::string name, std::string password);
		Channel &operator=(const Channel &op);
		~Channel();

		bool	getInviteOnly();
		bool	getTopicRestrict();
		int		getClientsMax();
		std::string	getName();
		std::string	getPassword();
		std::string	getTopic();

		void	setTopic(std::string topic);
		void	setTopicRestrict(bool topicRestrict);
		void	setInviteOnly(bool inviteOnly);
		void	setClientsMax(int clientsMax);

		void	addClient(Client *client);

}; 

#endif

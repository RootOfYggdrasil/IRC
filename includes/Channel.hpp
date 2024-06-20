#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Command.hpp"
# include "Utils.hpp"

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
		std::map<std::string, Client*>	_invitedClients;
		std::map<std::string, Client*>	_clients;
		std::map<std::string, Client*>	_operatorClients;
		//clients
		//operatori

	public:
		Channel();
		Channel(std::string name);
		Channel(std::string name, std::string password);
		Channel(std::string name, std::string password, Client &client);
		Channel &operator=(const Channel &op);
		~Channel();

		bool	getInviteOnly();
		bool	getTopicRestrict();
		int		getClientsMax();
		std::string	getName();
		std::string	getPassword();
		std::string	getTopic();
		std::map<std::string, Client*> &getClients();
		std::vector<Client*>	getLoggedClients();
		std::vector<Client*>	getLoggedClients(Client &client);
		std::vector<Client*>	getOperatorClients();

		void	setTopic(std::string topic);
		void	setTopic(std::vector<std::string> &vArguments);
		void	setTopicRestrict(bool topicRestrict);
		void	setInviteOnly(bool inviteOnly);
		void	setClientsMax(int clientsMax);
		void	setPassword(std::string password);
		void	setName(std::string name);


		bool	isOperator(Client &client);
		bool	isOperator(std::string nickname);
		bool	isClientInvited(Client &client);
		void	addClient(Client *client);
		void 	sendToAllClients(const std::string &msg);
		void	deleteClient(Client *client);
		void	deleteClient(std::string nickname);
		void	deleteOperatorClient(Client *client);
		void	deleteOperatorClient(std::string nickname);
		void	addOperatorClient(Client *client);
		void	addOperatorClient(std::string nickname);
		void	addInvitedClient(Client *client);
		void	removeInvitedClient(Client *client);
		void	removeInvitedClient(std::string nickname);

		bool	isClientOnChannel(std::string nickname);
		int		getClientCount();
		Client	getClient(std::string nickname);
		Client	*getClientPtr(std::string nickname);
};

std::ostream &operator<<(std::ostream &o, Channel &ch);
std::ostream &operator<<(std::ostream &o, Channel *ch);

#endif

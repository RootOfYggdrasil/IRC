
#include "../includes/Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name)
{
	this->_name = toLowerString(name);
	this->_password = "";
	this->_topic = "";
	this->_creationTime = "";
	this->_inviteOnly = false;
	this->_topicRestrict = false;
	this->_clientsMax = 0;
}

Channel::Channel(std::string name, std::string password)
{
	this->_name = toLowerString(name);
	this->_password = password;
	this->_topic = "";
	this->_creationTime = "";
	this->_inviteOnly = false;
	this->_topicRestrict = false;
	this->_clientsMax = 0;
}

Channel::Channel(std::string name, std::string password, Client &client)
{
	this->_name = toLowerString(name);
	this->_password = password;
	this->_topic = "";
	this->_creationTime = "";
	this->_inviteOnly = false;
	this->_topicRestrict = false;
	this->_clientsMax = 0;
	this->_clients[client.getNickname()] = &client;
	this->_operatorClients[client.getNickname()] = &client;
}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel &op)
{
	this->_name = op._name;
	this->_password = op._password;
	this->_topic = op._topic;
	this->_creationTime = op._creationTime;
	this->_inviteOnly = op._inviteOnly;
	this->_topicRestrict = op._topicRestrict;
	this->_clientsMax = op._clientsMax;
	return *this;
}

bool	Channel::getInviteOnly()
{
	return (this->_inviteOnly);
}

bool	Channel::getTopicRestrict()
{
	return (this->_topicRestrict);
}

int		Channel::getClientsMax()
{
	return (this->_clientsMax);
}

std::string	Channel::getName()
{
	return (this->_name);
}

std::string	Channel::getPassword()
{
	return (this->_password);
}

std::string	Channel::getTopic()
{
	return (this->_topic);
}

std::map<std::string, Client*> &Channel::getClients()
{
	return (this->_clients);
}

std::vector<Client*>	Channel::getLoggedClients()
{
	std::vector<Client*>	vLoggedClients;

	for (std::map<std::string, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getIsLogged())
			vLoggedClients.push_back(it->second);
	}
	return (vLoggedClients);
}

//ignoring the client that is sending the message
std::vector<Client*>	Channel::getLoggedClients(Client &client)
{
	std::vector<Client*>	vLoggedClients;

	for (std::map<std::string, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getIsLogged() && it->second->getNickname() != client.getNickname())
			vLoggedClients.push_back(it->second);
	}
	return (vLoggedClients);
}

std::vector<Client*>	Channel::getOperatorClients()
{
	std::vector<Client*>	vOperatorClients;

	for (std::map<std::string, Client*>::iterator it = this->_operatorClients.begin(); it != this->_operatorClients.end(); it++)
	{
		if (this->isOperator(*it->second))
			vOperatorClients.push_back(it->second);
	}
	return (vOperatorClients);
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void	Channel::setTopic(std::vector<std::string> &vArguments)
{
	if (vArguments.size() > 1)
	{
		this->_topic = vArguments[1];
		for (size_t i = 2; i < vArguments.size(); i++)
			this->_topic += " " + vArguments[i];
	}
}

void	Channel::setTopicRestrict(bool topicRestrict)
{
	this->_topicRestrict = topicRestrict;
}

void	Channel::setInviteOnly(bool inviteOnly)
{
	this->_inviteOnly = inviteOnly;
}

void	Channel::setClientsMax(int clientsMax)
{
	this->_clientsMax = clientsMax;
}

bool	Channel::isOperator(Client &client)
{
	return (this->_operatorClients.find(client.getNickname()) != this->_operatorClients.end());
}

void	Channel::addClient(Client *client)
{
	if (client && this->_clients.find(client->getNickname()) == this->_clients.end())
		this->_clients[client->getNickname()] = client;
}

void	Channel::sendToAllClients(const std::string &msg)
{
	/*for (std::map<std::string, Client *>::iterator	it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->second)
			send(it->second->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}*/
	std::vector<Client *> allLoggedClient = this->getLoggedClients();
	for (std::vector<Client *>::iterator it = allLoggedClient.begin(); it != allLoggedClient.end(); ++it)
		send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

void	Channel::deleteClient(Client *client)
{
	std::map<std::string, Client *>::iterator it = this->_clients.find(client->getNickname());
	if (it != this->_clients.end())
		this->_clients.erase(it);
}

void	Channel::deleteClient(std::string nickname)
{
	std::map<std::string, Client *>::iterator it = this->_clients.find(nickname);
	if (it != this->_clients.end())
		this->_clients.erase(it);
}



#include "../includes/Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name)
{
	this->_name = name;
	this->_password = "";
	this->_topic = "";
	this->_creationTime = "";
	this->_inviteOnly = false;
	this->_topicRestrict = false;
	this->_clientsMax = 0;
}

Channel::Channel(std::string name, std::string password)
{
	this->_name = name;
	this->_password = password;
	this->_topic = "";
	this->_creationTime = "";
	this->_inviteOnly = false;
	this->_topicRestrict = false;
	this->_clientsMax = 0;
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
	return (this->_clients.find(client.getNickname()) != this->_clients.end());
}

void	Channel::addClient(Client *client)
{
	if (client && this->_clients.find(client->getNickname()) == this->_clients.end())
		this->_clients[client->getNickname()] = client;
}



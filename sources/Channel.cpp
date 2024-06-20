
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

void	Channel::setPassword(std::string password)
{
	this->_password = password;
}

void	Channel::setName(std::string name)
{
	this->_name = name;
}

bool	Channel::isOperator(Client &client)
{
	return (this->_operatorClients.find(client.getNickname()) != this->_operatorClients.end());
}

bool	Channel::isOperator(std::string nickname)
{
	return (this->_operatorClients.find(nickname) != this->_operatorClients.end());
}

bool	Channel::isClientInvited(Client &client)
{
	return (this->_invitedClients.find(client.getNickname()) != this->_invitedClients.end());
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
	{
		this->_clients.erase(it);
	}
}

bool	Channel::isClientOnChannel(std::string nickname)
{
	return (this->_clients.find(nickname) != this->_clients.end());
}

int		Channel::getClientCount()
{
	return (this->_clients.size());
}

Client	Channel::getClient(std::string nickname)
{
	std::map<std::string, Client *>::iterator it = this->_clients.find(nickname);
	if (it != this->_clients.end())
		return (*it->second);
	return (Client());
}

Client	*Channel::getClientPtr(std::string nickname)
{
	std::map<std::string, Client *>::iterator it = this->_clients.find(nickname);
	if (it != this->_clients.end())
		return (it->second);
	return (NULL);
}

void	Channel::deleteOperatorClient(Client *client)
{
	std::map<std::string, Client *>::iterator it = this->_operatorClients.find(client->getNickname());
	if (it != this->_operatorClients.end())
		this->_operatorClients.erase(it);
}

void	Channel::deleteOperatorClient(std::string nickname)
{
	std::map<std::string, Client *>::iterator it = this->_operatorClients.find(nickname);
	if (it != this->_operatorClients.end())
		this->_operatorClients.erase(it);
}

void	Channel::addOperatorClient(Client *client)
{
	if (client && this->_operatorClients.find(client->getNickname()) == this->_operatorClients.end())
		this->_operatorClients[client->getNickname()] = client;
}

void	Channel::addOperatorClient(std::string nickname)
{
	if (this->_clients.find(nickname) != this->_clients.end())
		this->_operatorClients[nickname] = this->_clients[nickname];
}

void	Channel::addInvitedClient(Client *client)
{
	if (client && this->_invitedClients.find(client->getNickname()) == this->_invitedClients.end())
		this->_invitedClients[client->getNickname()] = client;
}

void	Channel::removeInvitedClient(Client *client)
{
	std::map<std::string, Client *>::iterator it = this->_invitedClients.find(client->getNickname());
	if (it != this->_invitedClients.end())
		this->_invitedClients.erase(it);
}

std::ostream &operator<<(std::ostream &o, Channel &ch)
{
	o << "Channel:" << std::endl;
	o << "  name: " << ch.getName() << std::endl;
	o << "  password: " << ch.getPassword() << std::endl;
	o << "  topic: " << ch.getTopic() << std::endl;
	o << "  inviteOnly: " << ch.getInviteOnly() << std::endl;
	o << "  topicRestrict: " << ch.getTopicRestrict() << std::endl;
	o << "  clientsMax: " << ch.getClientsMax() << std::endl;
	return o;
}

std::ostream &operator<<(std::ostream &o, Channel *ch)
{
	o << "Channel:" << std::endl;
	o << "  name: " << ch->getName() << std::endl;
	o << "  password: " << ch->getPassword() << std::endl;
	o << "  topic: " << ch->getTopic() << std::endl;
	o << "  inviteOnly: " << ch->getInviteOnly() << std::endl;
	o << "  topicRestrict: " << ch->getTopicRestrict() << std::endl;
	o << "  clientsMax: " << ch->getClientsMax() << std::endl;
	return o;
}

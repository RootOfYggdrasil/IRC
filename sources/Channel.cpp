
#include "../includes/Channel.hpp"

Channel::Channel() {}

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

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
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






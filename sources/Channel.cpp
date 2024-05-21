
#include "../includes/Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel &op)
{

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





#include "../includes/Client.hpp"

Client::Client(void) : _clientFd(-1), _nickname("BOT_DELPIPO"), _user("BOT-el-pipon"), _isLogged(true), _hasPw(true) {}

Client::Client(int fd) : _clientFd(fd), _nickname(""), _user(""), _isLogged(false), _hasPw(false){}

Client& Client::operator=(const Client &op)
{
	_clientFd = op._clientFd;
	return *this;
}

Client::~Client(){}

int Client::getFd() const {return this->_clientFd;}
bool Client::getIsLogged() const {return this->_isLogged;}
std::string  Client::getNickname() const {return this->_nickname;}
std::string  Client::getBuffer() const {return this->_buffer;}
bool Client::getPw() const {return this->_hasPw;}
const std::string &Client::getUsername() const {return this->_user;}
Channel *Client::getChannel(const std::string &channelName) const
{
	if(this->_connectedOnChannel.find(channelName) != this->_connectedOnChannel.end())
		return this->_connectedOnChannel.at(channelName);
	return NULL;
}

void Client::setIsLogged(bool logged) {_isLogged = logged;}
void Client::setNikcname(const std::string nickname){this->_nickname = nickname;}
void Client::setBuffer(std::string buffer) {this->_buffer = buffer;}
void Client::setPw(bool pw) {this->_hasPw = pw;}
void Client::setUsername(std::string &user) {this->_user = user;}

void Client::addChannel(Channel *channel)
{
	if(channel && this->_connectedOnChannel.find(channel->getName()) == _connectedOnChannel.end())
		this->_connectedOnChannel[channel->getName()] = channel;
}

void Client::deleteChannelJoined(Channel *ch)
{
	if (ch)
	{
		std::map<std::string, Channel *>::iterator it = _connectedOnChannel.find(ch->getName());
		if (it != _connectedOnChannel.end())
			_connectedOnChannel.erase(it);
	}
}
/*
void Client::deleteClFromCh(Server &server)
{
	for (std::map<std::string, Channel *>::iterator it = _connectedOnChannel.begin(); it != _connectedOnChannel.end(); it++)
	{
		it->second->deleteClient(this);
		if (it->second->getClients().size() == 0)
			server.deleteChannel(it->second);
	}
}*/


std::ostream &operator<<(std::ostream &o, const Client &cl)
{
	o << "Client:" << std::endl;
	o << "  fd: " << cl.getFd() << std::endl;
	o << "  nickname: " << cl.getNickname() << std::endl;
	o << "  user: " << cl.getUsername() << std::endl;
	o << "  isLogged: " << cl.getIsLogged() << std::endl;
	o << "  hasPw: " << cl.getPw() << std::endl;
	return o;
}

std::ostream &operator<<(std::ostream &o, const Client *cl)
{
	o << "Client:" << std::endl;
	o << "  fd: " << cl->getFd() << std::endl;
	o << "  nickname: " << cl->getNickname() << std::endl;
	o << "  user: " << cl->getUsername() << std::endl;
	o << "  isLogged: " << cl->getIsLogged() << std::endl;
	o << "  hasPw: " << cl->getPw() << std::endl;
	return o;
}
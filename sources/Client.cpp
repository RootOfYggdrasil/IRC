
#include "../includes/Client.hpp"

Client::Client(void) : _clientFd(-1), _nickname("redelpipo"), _user("el-pipon"), _isLogged(false) {}

Client::Client(int fd) : _clientFd(fd), _nickname(""), _user(""), _isLogged(false) {}

Client& Client::operator=(const Client &op)
{
	_clientFd = op._clientFd;
	return *this;
}

Client::~Client(){}

const int Client::getFd() const {return this->_clientFd;}
bool Client::getIsLogged() const {return this->_isLogged;}
std::string  Client::getNickname() const {return this->_nickname;}
std::string  Client::getBuffer() const {return this->_buffer;}
bool Client::getPw() const {return this->_hasPw;}
const std::string &Client::getUsername() const {return this->_user;}

void Client::setIsLogged(bool logged) {_isLogged = logged;}
void Client::setNikcname(const std::string &nickname){this->_nickname = nickname;}
void Client::setBuffer(std::string buffer) {this->_buffer = buffer;}
void Client::setPw(bool pw) {this->_hasPw = pw;}
void Client::setUsername(std::string &user) {this->_user = user;}

void Client::addChannel(Channel *channel)
{
	if(channel && this->_connectedOnChannel.find(channel->getName()) == _connectedOnChannel.end())
		this->_connectedOnChannel[channel->getName()] = channel;
}

#include "../includes/Client.hpp"

Client::Client(void) : _clientFd(-1), _nickname("redelpipo"), _user("el-pipon"), _isLogged(false) {}

Client::Client(int fd) : _clientFd(fd), _nickname(""), _user(""), _isLogged(false) {}

Client& Client::operator=(const Client &op)
{
	_clientFd = op._clientFd;
	return *this;
}

Client::~Client(){}

const int &Client::getFd() const {return this->_clientFd;}
bool Client::getIsLogged() const {return this->_isLogged;}
std::string  Client::getNickname() const {return this->_nickname;}
std::string  Client::getBuffer() const {return this->_buffer;}
bool Client::getHasPw() const {return this->_hasPw;}

void Client::setIsLogged(bool logged){_isLogged = logged;}
void Client::setBuffer(std::string &buffer){this->_buffer = buffer;}

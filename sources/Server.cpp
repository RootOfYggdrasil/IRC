
#include "../includes/Server.hpp"

Server::Server(void) : _port(0), _socket(0), _password("") { }

Server::Server(int port, std::string password) : _port(port), _socket(0), _password(password) { }

Server::~Server(void) { }

Server::Server(const Server &copy) : _port(copy._port), _socket(copy._socket), _password(copy._password) { }

Server &Server::operator=(const Server &op)
{
	if (this == &op)
		return (*this);
	_port = op._port;
	_socket = op._socket;
	_password = op._password;
	return (*this);
}

int Server::getPort() const { return this->_port; }

int Server::getSocket() const { return this->_socket; }

void Server::Start(void)
{
	struct sockaddr_in addr;
	struct epoll_event ev;
	int e=1;

	std::cout << "Server started on port " << this->_port << std::endl;
	std::cout << "Password: " << this->_password << std::endl;
	std::cout << "Socket: " << this->_socket << std::endl;
	std::cout << "WELCOME TO SERVER" << std::endl;

	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(this->_socket == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		exit(1);
	}
	setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &e, sizeof(int));
	fcntl(this->_socket, F_SETFL, O_NONBLOCK);
	bind(this->_socket, (struct sockaddr *)&addr, sizeof(addr));
	listen(this->_socket, 10);
	ev.events = EPOLLIN;
	ev.data.fd = this->_socket;
	ev.data.ptr = NULL;
}



#include "../includes/Server.hpp"

Server::Server(void) : _port(0), _socket(0), _password("") { }

Server::Server(int port, std::string password) : _port(port), _socket(0), _password(password) 
{
}

Server::~Server(void) { }

//Server::Server(const Server &copy) : _port(copy._port), _socket(copy._socket), _password(copy._password) { }

int Server::getPort() const { return this->_port; }

int Server::getSocket() const { return this->_socket; }

void	Server::Initialize(void)
{
	struct epoll_event ev;
	int e=1;

	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(this->_socket == -1)
		throw std::runtime_error("ERROR: socket creation failed");
	if(setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &e, sizeof(int)) == -1)
		throw std::runtime_error("ERROR: setsockopt enable failed");

	bind(this->_socket, (struct sockaddr *)&this->_addr, sizeof(this->_addr));//che fa?
	
	if(listen(this->_socket, 1000) == -1)
		throw std::runtime_error("ERROR: listen failed");
	
	/*ev.events = EPOLLIN;
	ev.data.fd = this->_socket;
	ev.data.ptr = NULL;
	*/
	this->_epollFD = epoll_create1(0);
	if(this->_epollFD == -1)
		throw std::runtime_error("ERROR: epoll create failed");	
	ev.events = EPOLLIN;
	ev.data.fd = this->_socket;
	if(epoll_ctl(this->_epollFD, EPOLL_CTL_ADD, this->_socket, &ev) == -1)
		throw std::runtime_error("ERROR: epoll ctl failed");

	this->_isInitialized = true;

	std::cout << "Server Initialized on port " << this->_port << std::endl;
	std::cout << "Password: " << this->_password << std::endl;
	std::cout << "Socket: " << this->_socket << std::endl;
}

void	Server::Run(void)
{
	if (!this->_isInitialized)	
		throw std::runtime_error("ERROR: server not initialized");
	this->_isRunning = true;
	std::cout << "Server is UP" << std::endl;
	while (this->_isRunning)
	{
		int	eventNumber = epoll_wait(this->_epollFD, this->_events, 1000, -1);
		//detect new events
		//Accept();
		//Receive();
		//Send();
		std::cout << "Event Detected! Nr: " << eventNumber << std::endl;
	}
	//Close();
}


#include "../includes/Server.hpp"

char hostname[1024]{};

Server::Server(void) : _port(0), _serverSocket(0), _password("") { }

Server::Server(int port, std::string password) : _port(port), _serverSocket(0), _password(password) 
{
}

Server::Server(Server const &src) 
{
	*this = src;
}

Server &Server::operator=(Server const &op)
{
	if (this != &op)
	{
		//this->_port = op._addr;
		this->_serverSocket = op._serverSocket;
		this->_epollFD = op._epollFD;
		this->_ip = op._ip;
		this->_password = op._password;
	}
	return *this;
}

Server::~Server(void) { }


int Server::getPort() const { return this->_port; }
int Server::getSocket() const { return this->_serverSocket; }
std::string Server::getPw() const { return this->_password; }

void Server::setPort(const in_port_t &port){ this->_port = port; }
void Server::setSocket(int _serverSocketFd){ this->_serverSocket = _serverSocketFd; }
void Server::setPw(const std::string &pw){ this->_password = pw; }


bool Server::signal = false;
void Server::checkSignal(int signal)
{
	(void)signal;
	std::cout << std::endl << "Signal recieved" << std::endl;
	Server::signal = true;
}

void	Server::InitializeServer(void)
{
	int e = 1;

	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(this->_serverSocket == -1)
		throw std::runtime_error("ERROR: socket creation failed");
	if(setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &e, sizeof(int)) == -1)
		throw std::runtime_error("ERROR: setsockopt enable failed");
	if(fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK) == -1) //set socket option (O_NONBLOCK) for non-blocking socket
		throw std::runtime_error("ERROR: fcntl failed");
	if(gethostname(hostname, sizeof(hostname)) == -1)
		throw std::runtime_error("ERROR: gethostname failed");
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(this->_port);
	this->_addr.sin_addr.s_addr = *((in_addr_t *)gethostbyname(hostname)->h_addr_list[0]);
	//the bind function binds the socket to the address and port number specified in addr(custom data structure).
	bind(this->_serverSocket, (struct sockaddr *)&this->_addr, sizeof(this->_addr));
		//throw std::runtime_error("ERROR: bind failed");
	if(listen(this->_serverSocket, 100) == -1)
		throw std::runtime_error("ERROR: listen failed");
	/*strarting epoll*/
	this->_epollFD = epoll_create1(0);
	if(this->_epollFD == -1)
		throw std::runtime_error("ERROR: epoll creation failed");	
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_serverSocket;
	if(epoll_ctl(this->_epollFD, EPOLL_CTL_ADD, this->_serverSocket, &this->_ev) == -1)
		throw std::runtime_error("ERROR: epoll ctl failed");

	this->_isInitialized = true;
	std::cout << "Server Initialized on port " << this->_port << std::endl;
	std::cout << "Password: " << this->_password << std::endl;
	std::cout << "Socket: " << this->_serverSocket << std::endl;
}

void	Server::Run(void)
{
	if (!this->_isInitialized)
		throw std::runtime_error("ERROR: server not initialized");
	this->_isRunning = true;
	std::cout << "Server is UP" << std::endl;
	while (this->_isRunning)
	{
		int	eventNumber = epoll_wait(this->_epollFD, this->_events, 100, 0); //check non blocking events(0)
		if(eventNumber != -1)
		{
			for(int i = 0; i < eventNumber; i++)
			{
				if(this->_events[i].data.fd == this->_serverSocket) //if the event is on the server socket
				{
					int clientSocket = accept(this->_serverSocket, NULL, NULL);
					if(clientSocket == -1)
						throw std::runtime_error("ERROR: accept failed");
					if(fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
					{
						close(clientSocket);
						throw std::runtime_error("ERROR: fcntl failed");
					}
					this->_ev.events = EPOLLIN | EPOLLET;
					this->_ev.data.fd = clientSocket;
					if(epoll_ctl(this->_epollFD, EPOLL_CTL_ADD, clientSocket, &this->_ev) == -1)
						throw std::runtime_error("ERROR: epoll ctl failed");
					std::cout << "New Client Connected: " << clientSocket << std::endl;
				}
				else
				{
					
				}
			}
		}
		
		
		//detect new events
		//Accept(): extracts the first connection request on the queue of pending connections 
		//for the listening socket, sockfd, creates a new connected socket, and returns a new
		// file descriptor referring to that socket. At this point, the connection is established
		// between client and server, and they are ready to transfer data.
		//Receive();
		//Send();
		std::cout << "Event Detected! Nr: " << eventNumber << std::endl;
	}
	//Close();
}

void Server::fdClose(void)
{
	// for(size_t i = 0; i < clients.size(); i++){ //-> close all the clients
 	//  std::cout << "Client <" << clients[i].GetFd() << "> Disconnected" << std::endl;
  	// close(clients[i].GetFd());
	close(this->_serverSocket);
	close(this->_epollFD);
	this->_isRunning = false;
	this->_isInitialized = false;
	std::cout << "Server Closed" << std::endl;
}

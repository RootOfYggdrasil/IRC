
#include "../includes/Server.hpp"

char hostname[1024]{};

Server::Server(int port, std::string password) : _port(port), _serverSocket(0), _password(password) 
{
	_clients[]
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
	std::cout << std::endl << "Signal received" << std::endl;
	Server::signal = true;
	exit(1);
}

Client *Server::getClient(const std::string &nickname) const
{
	Client *cl = NULL;
	std::map<std::string, Client*>::const_iterator it = _clients.find(nickname);
	if (it != _clients.end())
		cl = it->second;
	return cl;
}

Channel *Server::getChannel(const std::string &channelName) const
{
	//da vedere se ci sono indicazioni da rispettare es toLower
	Channel *ch = NULL;
	std::map<std::string, Channel*>::const_iterator it = _channels.find(channelName);
	if (it != _channels.end())
		ch = it->second;
	return ch;
}

/*cerca un Client con un fd specificato prima nella mappa _clients e poi nella lista _newClient_toRegister. 
Se trova il client, restituisce il puntatore a quell'oggetto Client.
Se non trova nessun client con l'fd specificato, restituisce NULL.*/
Client* Server::getClientComparingfFd(int fd) const
{
    // Lambda function to compare fd
    auto compareFd = [fd](const Client* client) {
        return client && client->getFd() == fd;
    };

    // Search in the _clients map
    auto it = std::find_if(_clients.begin(), _clients.end(),
        [&compareFd](const std::pair<std::string, Client*>& pair) {
            return compareFd(pair.second);
        });
    
    if (it != _clients.end()) {
        return it->second;
    }

    // Search in the _clientsNotRegistered list
    auto lit = std::find_if(_newClient_toRegister.begin(), _newClient_toRegister.end(), compareFd);
    
    if (lit != _newClient_toRegister.end()) {
        return *lit;
    }

    return nullptr;
}

std::vector<std::string> splitCmd(std::string &line)
{
	std::vector<std::string> commands;
	std::string cmd;
	std::istringstream iss(line);
	while (std::getline(iss, cmd, ' '))
	{
		if (parma[0] == ':')
		{
			std::string last;
			std::getline(iss, last, (char)EOF);
			cmd.erase(0, 1);
			if (last.size() + cmd.size())
			{
				if (!cmd.empty())
					commands.push_back(cmd + " " + last);
				else
					commands.push_back(cmd);
			}
		}
		else
			commands.push_back(cmd);
	}
	if (commands.size())
	{
		size_t pos = commands[commands.size() - 2].find("\r\n");
		while (pos != std::string::npos)
		{
			commands[commands.size() - 2].erase(pos, 1);
			pos = commands[commands.size() - 2].find("\r\n");
		}
	}
	return commands;
}

void Server::handleCommand(Client &client, std::vector<std::string> commands)
{
	std::srting cmd = commands[0];
	std::map<std::string, Command>::iterator it = this->_commands.find(cmd);

	if (!commands.size())
		return;
	commands.erase(commands.begin());
	if (it != this->_commands.end())
	{
		this->_commands[cmd].execute(client, commands);
	}
}

void Server::handleMessage(Client &client, const char *msg)
{
	std::string message = msg;
	size_t pos = message.find("\r\n");
	if (pos != std::string::npos)
		message = message.substr(0, pos);
	message = client.getBuffer() + message;
	client.setBuffer("");
	while (pos != std::string::npos)
	{
		std::string line;
		std::isstringstrem iss(message);
		std::getline(iss, line);
		std::cout << "Line: " << line << std::endl;
		std::vector<std::string> commands = splitCmd(line);
		if (client.getIsLogged())
			handleCommand(client, commands);
		else 
		//DA FARE 
			registerNotLogged(client, commands);
		message.erase(0, pos + 1);
	}
	client.setBuffer(message);
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
	memset(&_ev, 0, sizeof(epoll_event));
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_serverSocket;
	if(epoll_ctl(this->_epollFD, EPOLL_CTL_ADD, this->_serverSocket, &this->_ev) == -1)
		throw std::runtime_error("ERROR: epoll ctl failed");
	_fdCounter++;	

	this->_isInitialized = true;
	std::cout << "Server Initialized on port " << this->_port << std::endl;
	std::cout << "Password: " << this->_password << std::endl;
	std::cout << "Socket: " << this->_serverSocket << std::endl;
}

void	Server::Run(void)
{
	_fdCounter = 1;

	if (!this->_isInitialized)
		throw std::runtime_error("ERROR: server not initialized");
	this->_isRunning = true;
	std::cout << "Server is UP" << std::endl;
	while (this->_isRunning)
	{
		int	eventNumber = epoll_wait(this->_epollFD, this->_maxEvents, 100, 0); //check non blocking events(0)
		if(eventNumber > 0)
		{
			std::cout << "Event Detected! Nr: " << eventNumber << std::endl;
			for(int i = 0; i < eventNumber; i++)
			{
				//if the event is on the server socket
				if(this->_maxEvents[i].data.fd == this->_serverSocket) 
				{
					int newFdSocket = accept(this->_serverSocket, NULL, NULL);
					
					if(_fdCounter >= 1024 - 1)
					{
						send(newFdSocket, ":ircserv QUIT :The server is full!\r\n", 37, MSG_DONTWAIT | MSG_NOSIGNAL);
						send(newFdSocket, "", 0, MSG_DONTWAIT | MSG_NOSIGNAL);
						close(newFdSocket);
						continue;
					}
					if(newFdSocket == -1)
						throw std::runtime_error("ERROR: accept failed");
					if(fcntl(newFdSocket, F_SETFL, O_NONBLOCK) == -1)
						throw std::runtime_error("ERROR: fcntl failed");
					this->_ev.events = EPOLLIN | EPOLLET;
					this->_ev.data.fd = newFdSocket;
					if(epoll_ctl(this->_epollFD, EPOLL_CTL_ADD, newFdSocket, &this->_ev) == -1)
						throw std::runtime_error("ERROR: epoll ctl failed");
					std::cout << "New Client Connected: " << newFdSocket << std::endl;
					_fdCounter++;
					
					//fd in non registered client list
					Client* newClient = new Client(newFdSocket);
					if (newClient != 0)
					{
						std::string	RPL_INFO = ":ircserv INFO :Connected to 42IRC server!\r\n";
						send(newFdSocket, RPL_INFO.c_str(), RPL_INFO.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
						_newClient_toRegister.push_back(newClient);
					}
				}
				// else if the event is NOT on the server socket
				else
				{
					char buff[512];

					Client* newCL = NULL;
					int clientFdSocket = _maxEvents[i].data.fd;
					newCL = getClientComparingfFd(clientFdSocket);
					memset(buff, 0, sizeof(buff));
					int bytes = recv(clientFdSocket, buff, sizeof(buff), 0);
					if(bytes == -1)
						throw std::runtime_error("ERROR: recv failed");
					else if(bytes == 0)
					{
						std::cout << "Client <"	<< clientFdSocket << "> Disconnected" << std::endl;
						close(clientFdSocket);
						epoll_ctl(this->_epollFD, EPOLL_CTL_DEL, clientFdSocket, NULL);
						_fdCounter--;
						//remove client from _clients ma
						if (newCL != NULL)
						{
							//verificare se fare funzione apposita che elimini anche dal channel
							_clients.erase(newCL->getNickname());
							delete newCL;
						}
					else
					{
						std::string message = buff;
						std::cout << "Message from client <" << clientFdSocket << ">: " << message << std::endl;
						handleMessage(newCL, message);
					}
					
		


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


#include "../includes/Server.hpp"

char hostname[1024] = {};

Server::Server(in_port_t port, std::string password) : _port(port), _password(password), _serverSocket(0), _fdCounter(4), _okPw(password.compare("") != 0)
{
	srand(time(0));
	_commands["QUIT"] = &Command::quit;
	_commands["TOPIC"] = &Command::topic;
	_commands["KICK"] = &Command::kick;
	_commands["MODE"] = &Command::mode;
	_commands["INVITE"] = &Command::inv;
	_commands["JOIN"] = &Command::join;
	_commands["PRIVMSG"] = &Command::privmsg;
	_commands["NICK"] = &Command::nick;
	_commands["BOT"] = &Command::bot;
	_commands["PASS"] = &Command::password;
	Channel *welcome = new Channel("#welcome");
	addChannel(welcome);
	Client *bot = new Client();
	this->_clients[bot->getNickname()] = bot;
	welcome->addClient(bot);
	welcome->addOperatorClient(bot);

}

Server::~Server(void) { }


int Server::getPort() const { return this->_port; }
int Server::getSocket() const { return this->_serverSocket; }
const std::string Server::getPw() const { return this->_password; }

void Server::setPort(const in_port_t &port){ this->_port = port; }
void Server::setSocket(int _serverSocketFd){ this->_serverSocket = _serverSocketFd; }
//void Server::setPw(const std::string &pw){ this->_password = pw; }



Client *Server::getClient(const std::string &nickname) const
{
	Client *cl = NULL;
	std::map<std::string, Client*>::const_iterator it = this->_clients.find(nickname);
	if (it != this->_clients.end())
		cl = it->second;
	return cl;
}

Channel *Server::getChannel(const std::string &channelName) const
{
	Channel *ch = NULL;
	std::map<std::string, Channel*>::const_iterator it = this->_channels.find(toLowerString(channelName));
	if (it != _channels.end())
		ch = it->second;
	return ch;
}


void	Server::deleteChannel(const std::string &nameChannel)
{
	std::map<std::string, Channel *>::iterator	it = _channels.find(toLowerString(nameChannel));

	if (it != _channels.end())
	{
		delete(it->second);
		_channels.erase(it);
	}
}

/*cerca un Client con un fd specificato prima nella mappa _clients e poi nella lista _newClient_toRegister.
Se trova il client, restituisce il puntatore a quell'oggetto Client.
Se non trova nessun client con l'fd specificato, restituisce NULL.*/
Client* Server::getClientComparingfFd(int file) const {

    // Search in the _clients map
    std::map<std::string, Client*>::const_iterator itstart = this->_clients.begin();

	while (itstart != this->_clients.end()) {
		if (itstart->second && itstart->second->getFd() == file)
       		return itstart->second;
		++itstart;
    }

    // Search in the _newCltoRegister list
    std::list<Client*>::const_iterator litstart = this->_newCltoRegister.begin();

    while (litstart != this->_newCltoRegister.end()) {
		if (*litstart && (*litstart)->getFd() == file)
        	return *litstart;
		++litstart;
    }
	// Not found
    return NULL;
}

std::vector<std::string> Server::splitCmd(const std::string &line)
{
	std::vector<std::string> commands;
	std::string cmd;
	std::istringstream iss(line);
	while (std::getline(iss, cmd, ' '))
	{
		if (cmd.empty())
			continue;
		else if (cmd[0] == ':')
		{
			std::string last;
			std::getline(iss, last,(char)EOF);
			cmd.erase(0, 1);
			if (last.size() + cmd.size())
			{
				if (!cmd.empty())
				{
					last.size() == 0 ? commands.push_back(cmd) : commands.push_back(cmd + " " + last);
				}
				// if (!last.empty())
				// 	commands.push_back(cmd + " " + last);
				else
					commands.push_back(cmd);
			}
		}
		else
			commands.push_back(cmd);
	}
	if (commands.size())
	{
		size_t pos = commands[commands.size() - 1].find('\r');
		while (pos != std::string::npos)
		{
			commands[commands.size() - 1].erase(pos, 1);
			pos = commands[commands.size() - 1].find('\r');
		}
	}
	return commands;
}

void Server::handleCommand(Client &client, std::vector<std::string> pVector)
{
	std::string cmd = toUpperString(pVector[0]);
	std::map<std::string, functionCmd>::iterator it = this->_commands.find(toUpperString(cmd));

	if (!pVector.size())
		return;
	pVector.erase(pVector.begin());
	if (it != this->_commands.end())
	{
		this->_commands[cmd](*this, client, pVector);
	}
}

void Server::registerNotLogged(Client &client, std::vector<std::string> pVector)
{
	std::string cmd;

	if (!pVector.size())
		return;
	cmd = pVector[0];
	pVector.erase(pVector.begin());
	if ((!client.getPw()) && !cmd.compare("PASS"))
	 	Command::password(*this, client, pVector);
	else if (client.getPw() && !cmd.compare("NICK"))
	 	Command::nick(*this, client, pVector);
	else if (client.getPw() && !cmd.compare("USER"))
		Command::user(*this, client, pVector);
	else {
	 	std::string error = "451 " + client.getNickname() + " :You have not registered\r\n";
	 	std::cout << "Error: " << error << std::endl;
	 	send(client.getFd(), error.c_str(), error.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	  	return ;
	}
	std::cout << "Client " << client.getNickname() << " user " << client.getUsername() << std::endl;
	if (!client.getNickname().empty() && !client.getUsername().empty() && client.getPw())
	{
		if(this->getClient(client.getNickname())) {
			std::string error = "433 " + client.getNickname() + " :Nickname already in use\r\n";
			send(client.getFd(), error.c_str(), error.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
			client.setNikcname("");
			return ;
		}
		std::cout << client.getNickname() << " now has registered." << std::endl;

		std::string welcome = ":ircserv 001 " + client.getNickname() + " :Welcome to the 42 IRC Network " + client.getNickname() + "!\r\n";
		client.setIsLogged(true);
		this->_newCltoRegister.remove(&client);
		this->_clients[client.getNickname()] = &client;
		std::string superchannel = "#welcome";
		std::vector<std::string> welcomeVector;
		welcomeVector.push_back("JOIN");
		welcomeVector.push_back(superchannel);
		//sendToAll
		handleCommand(client, welcomeVector);
	}
}

void Server::handleMessage(Client &client, const char *msg)
{
	std::string message = msg;
	size_t pos = message.find('\n');
	if (pos != std::string::npos)
	 	message = message.substr(0, pos);
	message = client.getBuffer() + msg;
	client.setBuffer("");
	while (pos != std::string::npos)
	{
		std::string line;
		std::istringstream iss(message);
		std::getline(iss, line);
		std::cout << "Line: " << line << std::endl;
		std::vector<std::string> commands = splitCmd(line);
		if (commands.size() == 0)
			break;
		commands[0] = toUpperString(commands[0]);
		if (client.getIsLogged())
		{
			handleCommand(client, commands);
		}
		else
		{
			registerNotLogged(client, commands);
		}
		message.erase(0, pos + 1);
		pos = message.find('\n');
	}
	client.setBuffer(message);
}

void	Server::InitializeServer(void)
{
	int e = 1;

	signal(SIGINT, checkSignal);
	signal(SIGQUIT, checkSignal);

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
	if (bind(this->_serverSocket, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1)
		throw std::runtime_error("ERROR: bind failed");
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
	std::cout << "Hostname: " << hostname << std::endl;
	std::cout << "EpollFD: " << this->_epollFD << std::endl;
	std::cout << "FdCounter: " << this->_fdCounter << std::endl;
}

void	Server::Run()
{
	int newFdSocket = 0;

	if (!this->_isInitialized)
		throw std::runtime_error("ERROR: server not initialized");
	ServerisRunning = true;
	std::cout << "Server is UP" << std::endl;
	while (ServerisRunning)
	{
		int	eventNumber = epoll_wait(this->_epollFD, this->_maxEvents, 100, -1); //check non blocking events(0)
		if(eventNumber > 0)
		{
			std::cout << "Event Detected! Nr: " << eventNumber << std::endl;
			for(int i = 0; i < eventNumber; i++)
			{
				//if the event is on the server socket
				if(this->_maxEvents[i].data.fd == this->_serverSocket)
				{
					newFdSocket = accept(this->_serverSocket, NULL, NULL);

					if(this->_fdCounter >= 1024 - 1)
					{
						std::cout << "fdcounter: " << this->_fdCounter << std::endl;
						send(newFdSocket, ":ircserv QUIT :The server is full!\r\n", 37, MSG_DONTWAIT | MSG_NOSIGNAL);
						send(newFdSocket, "", 0, MSG_DONTWAIT | MSG_NOSIGNAL);
						close(newFdSocket);
						continue;
					}
					if(fcntl(newFdSocket, F_SETFL, O_NONBLOCK) == -1)
						throw std::runtime_error("ERROR: fcntl failed");

					this->_ev.events = EPOLLIN | EPOLLET;
					this->_ev.data.fd = newFdSocket;
					if(epoll_ctl(this->_epollFD, EPOLL_CTL_ADD, newFdSocket, &this->_ev) == -1)
						throw std::runtime_error("ERROR: epoll ctl failed");
					std::cout << "New Client Connected: " << newFdSocket << std::endl;
					this->_fdCounter++;

					//fd of non registered client insert in the list
					Client *newClient = new Client(newFdSocket);
					if (newClient)
					{
						std::string	msgInfo = ":ircserv INFO :Connecting to 42IRC server!\r\n";
						send(newFdSocket, msgInfo.c_str(), msgInfo.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
						_newCltoRegister.push_back(newClient);
					}
				}
				// else if the event is NOT on the server socket
				else
				{
					char buff[512];
					Client *newCL;
					int clientFdSocket = _maxEvents[i].data.fd;
					newCL = getClientComparingfFd(clientFdSocket);
					memset(buff, 0, sizeof(buff));
					int incomingBytes = recv(clientFdSocket, buff, sizeof(buff), 0);
					if (buff[0])
						std::cout << "Message from client <" << clientFdSocket << ">: " << buff << std::endl;
					if(incomingBytes <= 0)
					{
						std::cout << "Disconnecting" << newCL << std::endl;
						epoll_ctl(this->_epollFD, EPOLL_CTL_DEL, clientFdSocket, NULL);
						this->_fdCounter--;
						close(clientFdSocket);
						if (newCL)
						{
							this->eraseClientFromAllChannels(*newCL);
							this->_newCltoRegister.remove(newCL);
							this->_clients.erase(newCL->getNickname());
							delete newCL;
						}
						//remove client from _clients map
					}
					else if (newCL)
					{
						handleMessage(*newCL, buff);
					}
				}
			}
			std::cout << std::endl;
		}
	}
	std::cout << "Server is deleting things" << std::endl;
	for(std::list<Client *>::iterator it = this->_newCltoRegister.begin(); it != this->_newCltoRegister.end(); ++it)
	{
		std::string msg = ":ircserv QUIT : Server not connect\r\n";
		std::cout << "Deleting client: " << (*it)->getNickname() << std::endl;
		send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		epoll_ctl(this->_epollFD, EPOLL_CTL_DEL, (*it)->getFd(), NULL);

		if((*it)->getFd() >= 0)
		{
			this->_fdCounter--;
			close((*it)->getFd());
		}
		delete *it;
	}
	for(std::map<std::string, Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		std::string msg = ":ircserv QUIT : Server not connect\r\n";
		std::cout << "Deleting client: " << it->second->getNickname() << std::endl;
		send(it->second->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		epoll_ctl(this->_epollFD, EPOLL_CTL_DEL, it->second->getFd(), NULL);
		if(it->second->getFd() >= 0)
		{
			close(it->second->getFd());
			this->_fdCounter--;
		}
		delete it->second;
	}
	for(std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		std::cout << "Deleting channel: " << it->second->getName() << std::endl;
		delete it->second;
	}
	close(this->_epollFD);
	close(this->_serverSocket);
}

void	Server::addChannel(Channel *channel)
{
	if (channel  && !getChannel(channel->getName()))
		_channels[channel->getName()] = channel;
}

void	Server::addClient(Client *client)
{
	if (client && !getClient(client->getNickname()))
		_clients[client->getNickname()] = client;
}

void Server::sendToAll(const std::string &msg)
{
	for (std::map<std::string, Client *>::iterator	it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->second)
			send(it->second->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
}

bool Server::existsClient(const std::string &nickname) const
{
	return (this->_clients.find(nickname) != this->_clients.end());
}

void Server::deleteAllChannels()
{
	for(std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		std::cout << "Deleting channel: " << it->second << std::endl;
		if (it->second)
			delete it->second;
	}
	_channels.clear();
}

void Server::deleteAllClients()
{
	for(std::map<std::string, Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second)
		{
			std::string msg = ":ircserv QUIT : Server not connect\r\n";
			std::cout << "Deleting client: " << it->second << std::endl;
			send(it->second->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
			delete it->second;
		}
	}
	_clients.clear();
}

void Server::eraseClientFromAllChannels(Client &client)
{
	for(std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second)
		{
			it->second->deleteClient(client.getNickname());
			it->second->deleteOperatorClient(client.getNickname());
		}
	}
}

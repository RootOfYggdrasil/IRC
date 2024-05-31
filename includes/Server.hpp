#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sstream>
# include <fcntl.h> 
# include <unistd.h> 
# include <csignal> 
# include <vector>
# include <sys/socket.h> 
# include <sys/types.h>
# include <netinet/in.h> 
# include <arpa/inet.h> 
# include <sys/epoll.h> 
# include <string>
# include <stdexcept>
# include <string.h>
# include <iomanip>
# include <netdb.h>
# include <stdio.h>
# include <list>
# include <list>
# include <map>
# include <vector>
# include <algorithm>
# include <vector>
# include <algorithm>


# include "Client.hpp"
# include "Command.hpp"
# include "Channel.hpp"

typedef void (*functionCmd)(Server &server, Client &client, std::vector<std::string> &vArguments);

class Server {

	private:
		in_port_t 	_port;
		std::string	_password;
		int 		_serverSocket;
		int 		_epollFD;
		unsigned long int _fdCounter;
		bool		_isRunning;
		bool		_isInitialized;
		bool		_okPw;
		static bool signal;
		//std::string	_ip;
		struct sockaddr_in _addr;
		struct epoll_event _ev;
		struct epoll_event _maxEvents[1024];
		struct epoll_event _runningEvent;

		std::list<Client*> _newCltoRegister;
		std::map <std::string, Client*>	_clients;
		std::map <std::string, Channel*> _channels;
		std::map <std::string, functionCmd>	_commands;
		
	public:
		Server(in_port_t port, std::string password);
		~Server();
		void InitializeServer();
		void Run();

		int getPort() const;
		int getSocket() const;
		const std::string getPw() const;


		void setPort(const in_port_t &port);
		void setSocket(int _serverSocketFd);

		Client *getClientComparingfFd(int fd) const;
		Client *getClient(const std::string &nickname) const;
		Channel *getChannel(const std::string &channelName) const;	

		void handleMessage(Client &client, const char *msg);
		std::vector<std::string> splitCmd(const std::string &line);
		void handleCommand(Client &client, std::vector<std::string> pVector);
		void registerNotLogged(Client &client, std::vector<std::string> pVector);

		static void checkSignal(int signal);
		void fdClose(void);
}; 


#endif





/*Un socket è un canale di comunicazione che consente il trasferimento di dati tra computer.
Creando un punto di connessione tra due computer, è possibile inviare e ricevere dati da 
questo punto. Ogni computer ha un indirizzo IP e un socket comunica tramite questo indirizzo 
IP su una porta specifica*/

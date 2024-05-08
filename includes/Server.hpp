#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <fcntl.h> 
# include <unistd.h> 
# include <csignal> 
# include <vector>
# include <sys/socket.h> 
# include <sys/types.h>
# include <netinet/in.h> 
# include <arpa/inet.h> 
# include <sys/epoll.h> 
# include <string.h>
# include <netdb.h>
# include <stdio.h>

//# include "Client.hpp"
//# include "Command.hpp"
//# include "Channel.hpp"

//class Client;

class Server {

	private:
		in_port_t 	_port;
		int 		_serverSocket;
		int 		_epollFD;
		bool		_isRunning;
		bool		_isInitialized;
		static bool signal;
		std::string	_ip;
		std::string	_password;
		struct sockaddr_in _addr;
		struct epoll_event _ev;
		struct epoll_event _events[100];
		struct epoll_event _runningEvent;



		//std::vector<Client> clients;
		Server(void);// ha senso? Non credo
	public:
		~Server();
		Server(int port, std::string password);
		Server(const Server &copy);
		Server &operator=(const Server &op);
		void InitializeServer();
		void Run();

		int getPort() const;
		int getSocket() const;
		std::string getPw() const;

		void setPort(const in_port_t &port);
		void setSocket(int _serverSocketFd);
		void setPw(const std::string &pw);
		static void checkSignal(int signal);
		void fdClose();
}; 


#endif





/*Un socket è un canale di comunicazione che consente il trasferimento di dati tra computer.
Creando un punto di connessione tra due computer, è possibile inviare e ricevere dati da 
questo punto. Ogni computer ha un indirizzo IP e un socket comunica tramite questo indirizzo 
IP su una porta specifica*/

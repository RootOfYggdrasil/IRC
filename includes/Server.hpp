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

//# include "Client.hpp"
//# include "Command.hpp"
//# include "Channel.hpp"

//class Client;

class Server {

	private:
		int 	_port;
		int 	_socket;
		int 	_epollFD;
		bool	_isRunning;
		bool	_isInitialized;
		std::string	_ip;
		std::string	_password;
		struct sockaddr_in _addr;
		struct epoll_event _events[1000];



		//std::vector<Client> clients;
		//Server(const Server &copy);
		//Server &operator=(const Server &op);
		Server(void);// ha senso? Non credo
	public:
		~Server();
		Server(int port, std::string password);
		void Initialize();
		void Run();

		int getPort() const;
		int getSocket() const;
}; 


#endif





/*Un socket è un canale di comunicazione che consente il trasferimento di dati tra computer.
Creando un punto di connessione tra due computer, è possibile inviare e ricevere dati da 
questo punto. Ogni computer ha un indirizzo IP e un socket comunica tramite questo indirizzo 
IP su una porta specifica*/

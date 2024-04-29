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

# include "Client.hpp"
# include "Command.hpp"
# include "Channel.hpp"

class Client;

class Server {

	private:
		int const _port;
		std::string const _password		
		std::vector<Client> clients;
	
	public:
		Server(void);
		Server(const Server &copy);
		Server &operator=(const Server &op);
		~Server(void);

}; 


#endif





/*Un socket è un canale di comunicazione che consente il trasferimento di dati tra computer.
Creando un punto di connessione tra due computer, è possibile inviare e ricevere dati da 
questo punto. Ogni computer ha un indirizzo IP e un socket comunica tramite questo indirizzo 
IP su una porta specifica*/

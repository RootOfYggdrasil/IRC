
#include "../includes/Command.hpp"

std::string	phrasePacciani[6] = {
	"Who was Pacciani",
	"Pacciani was a serial killer in Italy, he was a farmer and he killed 14 people in the 80s. He was called the Monster of Florence.",
	"Posso parlà da i banco? Come, io non ho diritto a parlare? Voglio la libertà per andare alla banca e alla posta",
	"Risorgeremo. Prima o Dopo",
	"Signor Vanni, che lavoro fa lei?» «Io sono stato a fà delle merende co i Pacciani no?",
	"Se ni mondo esistesse un po di bene e ognun si considerasse suo fratello, ci sarebbe meno pensieri e meno pene e il mondo ne sarebbe assai più bello"
};

std::string phrasePiccol[] = {
	"Solo per oggi ma a partire da domani",
	"Smettila di morire di fame a casa vieni al Piccol",
	"Calendario illustrato di padre Maronno, 4 euro",
	"Solo dopo la chiusura, guanti da piede 3 euro",
	"Tizio raffredda brodo, 66-67 euro",
	"Succo d'anatras, cinquenta euro",
	"Crociera a Cortina 2 milioni di euro"
};

std::string phraseRandom[]= {
	"Signora Nordberg, credo che riusciremo a salvare il braccio di suo marito. Dove vuole che glielo spediamo?",
	"-Ho passato una giornata meravigliosa. Sembra impossibile che ci siamo conosciuti solo ieri.\nEL_PIPO -Lo pensi veramente, Jane? Non è che lo dici solo perché ci siano scambiati i fluidi corporei?",
	"I suoi capelli avevano lo steso colore dell'oro nei dipinti antichi, aveva un vasto assortimento di curve, un set completo di curve e due gambe che avresti ciucciato e leccato un giorno intero. Mi lanciò un'occhiata che m'indurì tutto. Persino i miei pantaloni sembravano inamidati. L'affare si stava veramente ingrossando."
};

Command::Command(void) {}
Command::~Command(void) {}

std::string	toLowerStr(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

std::string extractChannelName(std::vector<Client *> clients, std::string append)
{
	std::string channelName = "";
	for (size_t i = 0; i < clients.size(); i++)
	{
		channelName += append + clients[i]->getNickname() + " ";
	}
	return channelName;
}

void	addClientToChannel(Server &server, Client &client, std::string channelName, std::string &password)
{
	std::string clientMsg = "";
	Channel *channel = server.getChannel(channelName);

	if (channel == NULL)
	{
		channel = new Channel(toLowerStr(channelName), password, client);
		server.addChannel(channel);
		client.addChannel(channel);
	}
		//tutti i check del canale
		//canale pieno
	if (channel->getPassword() == password)
	{
		if (channel->getInviteOnly() && !channel->isClientInvited(client))
		{
			clientMsg = "473 " + client.getNickname() + " " + channelName + " :Cannot join channel, InviteOnly channel!\r\n";
			send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		}
		else if (channel->getClientsMax() > 0 && channel->getClientCount() >= channel->getClientsMax())
		{
			clientMsg = "473 " + client.getNickname() + " " + channelName + " :Cannot join channel, Full channel!\r\n";
			send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		}
		else
		{
			channel->addClient(&client);
			client.addChannel(channel);
			std::string chName = channel->getName();
			std::string RPL_JOIN_TOCLIENT = ":" + client.getNickname() + "!" + client.getUsername() + "@ReDelPipo JOIN :" + chName + "\r\n";
			std::string RPL_NAMREPLY = ":ircserv 353 " + client.getNickname() + " = " + chName + " :" + extractChannelName(channel->getOperatorClients(), "@") + extractChannelName(channel->getLoggedClients(), "")  + "\r\n";
			std::string RPL_ENDOFNAMES = ":ircserv 366 " + client.getNickname() + " " + chName + " :End of NAMES list\r\n";
			std::cout << RPL_NAMREPLY << std::endl;
			std::string topic = channel->getTopic();
			channel->sendToAllClients(RPL_JOIN_TOCLIENT);
			if (!channel->getTopic().empty())
			{
				clientMsg = ":ircserv 332 " + client.getNickname() + " " + chName + " :" + channel->getTopic() + "\r\n";
				send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
			}
			send(client.getFd(), RPL_NAMREPLY.c_str(), RPL_NAMREPLY.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
			send(client.getFd(), RPL_ENDOFNAMES.c_str(), RPL_ENDOFNAMES.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
			if (channel->isClientInvited(client))
			{
				channel->removeInvitedClient(&client);
			}

		}
	}
	else
	{
		clientMsg = "475 " + client.getNickname() + " " + channelName + " :Cannot join channel\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
}

void	Command::bot(Server &s, Client &client, std::vector<std::string> &vArguments)
{
	(void)s;
	std::string replyMsg = "";

 	if (vArguments.size() < 1)
	{
 		//replyMsg = "461 " + client.getNickname() + " BOT :Not enough parameters\r\n";
		replyMsg = client.getNickname() + " EL_PIPO" + " Error: Usage: 'bot' + <option('truth'-'dare'-'Pacciani'-'Piccol')>;\r\n";
	}
	else
	{
		if(vArguments[0].compare("truth") == 0)
			replyMsg = ":" + client.getNickname() + " EL_PIPO " + "Tell me your worst secret\r\n";
		else if(vArguments[0].compare("dare") == 0)
			replyMsg = ":" + client.getNickname() + " EL_PIPO " +  "You have to buy me a coffe\r\n";
		else if (vArguments[0].compare("Pacciani") == 0)
			replyMsg = ":" + client.getNickname() + " EL_PIPO " +  phrasePacciani[rand() % 6] + "\r\n";
		else if (vArguments[0].compare("Piccol") == 0)
			replyMsg = ":" + client.getNickname() + " EL_PIPO " +  phrasePiccol[rand() % 7] + "\r\n";
		else if (vArguments[0].compare("random") == 0)
			replyMsg = ":" + client.getNickname() + " EL_PIPO " +  phraseRandom[rand() % 3] + "\r\n";
	}
	send(client.getFd(), replyMsg.c_str(), replyMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

void	Command::join(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	std::string clientMsg = "", password = "", channelName = "";
	size_t	vectorSize = vArguments.size();

	if (vectorSize == 0 || vectorSize > 2)
	{
		clientMsg = "461 " + client.getNickname() + " JOIN :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else if (vectorSize == 1 && vArguments[0][0] != '#')
	{
		clientMsg = "403 " + client.getNickname() + " " + vArguments[0] + " :No such channel\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else if (vectorSize == 1)
	{
		std::istringstream channels(vArguments[0]);
		while (std::getline(channels, channelName, ','))
		{
			std::string password = "";
			addClientToChannel(server, client, channelName, password);
		}
	}
	else if (vectorSize == 2)
	{
		std::istringstream channels(vArguments[0]);
		std::istringstream passwords(vArguments[1]);
		while (std::getline(channels, channelName, ','))
		{
			std::getline(passwords, password, ',');
			addClientToChannel(server, client, channelName, password);
		}
	}
}

void Command::password(Server &server, Client &client, std::vector<std::string> &v)
{
	std::string clientMsg = "";
 	if (v.size() < 1)
 		clientMsg = "461 " + client.getNickname() + " PASSWORD :Not enough parameters\r\n";
 	if(client.getIsLogged())
 		clientMsg= "462 " + client.getNickname() + " :Already registered\r\n";
 	if (v[0].compare(server.getPw()) != 0)
 		clientMsg = ":ircserv 464 " + client.getNickname() + " :Password incorrect\r\n";
	else
		client.setPw(true);
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void	sendMsgToChannel(Server &server, Client &client, std::string channelName, std::string msg)
{
	std::string clientMsg = "";
	Channel *channel = server.getChannel(channelName);
	if (channel == NULL)
		clientMsg = "403 " + client.getNickname() + " " + channelName + " :No such channel\r\n";
	else if (!channel->isClientOnChannel(client.getNickname()))
		clientMsg = "442 " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
	else
	{
		std::vector<Client *> clientToMsg = channel->getLoggedClients(client);
		for (size_t i = 0; i < clientToMsg.size(); i++)
		{
			clientMsg = ":" + client.getNickname() + "! PRIVMSG " + channelName + " :" + msg + "\r\n";
			send(clientToMsg[i]->getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		}
		return;
	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

}

void	sendMsgToClient(Server &server, Client &client, std::string target, std::string msg)
{
	std::string clientMsg = "";
	Client *targetClient = server.getClient(target);
	std::cout << "target: " << target << std::endl;
	if (!targetClient)
	{
		clientMsg = "401 " + client.getNickname() + " " + target + " :No such nick\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
		clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + " PRIVMSG " + target + " :" + msg + "\r\n";
		send(targetClient->getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}

}

void	Command::privmsg(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	std::string clientMsg = "";
	std::vector<std::string>	target;

	if (vArguments.size() < 2)
	{
		clientMsg = "461 " + client.getNickname() + " PRIVMSG :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		return;
	}
	target = split(vArguments[0], ',');
	for(size_t i = 0; i < target.size(); i++)
	{
		if(target[i][0] == '#')
			sendMsgToChannel(server, client, target[i], vArguments[1]);
		else
			sendMsgToClient(server, client, target[i], vArguments[1]);
	}
}

bool	isValidNick(std::string nickname)
{
	if (nickname.size() > 16)
		return false;
	if (nickname.find_first_of("!@#$%^*=+\\|;:'\",<.>/?()[]{}") != std::string::npos)
		return false;
	return true;
}

void	Command::nick(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	std::string clientMsg = "";
	if (vArguments.size() < 1)
		clientMsg = "461 " + client.getNickname() + " NICK :Not enough parameters\r\n";
	else if (!isValidNick(vArguments[0]))
		clientMsg = "432 " + client.getNickname() + " :Erroneus nickname\r\n";
	else if (server.getClient(vArguments[0]))
		clientMsg = "433 " + client.getNickname() + " :Nickname is already in use, try a different one.\r\n";
	else
	{
		client.setNikcname(vArguments[0]);
	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

void	Command::kick(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	Channel *channel;
	if (vArguments.size() < 2)
	{
		clientMsg = "461 " + client.getNickname() + " KICK :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		return;
	}
	channel = client.getChannel(vArguments[0]);
	if (!channel)
		clientMsg = "403 " + client.getNickname() + " " + vArguments[0] + " :No such channel\r\n";
	else if (!channel->isClientOnChannel(client.getNickname()))
		clientMsg = "ERROR " + client.getNickname() + " " + vArguments[0] + " :You're not on that channel\r\n";
	else if (!channel->isOperator(client))
		clientMsg = "482 " + client.getNickname() + " " + channel->getName() + "\r\n";
	else
	{
		Client *clientToKick = server.getClient(vArguments[1]);
		if (!clientToKick)
			clientMsg = "401 " + client.getNickname() + " " + vArguments[1] + " :No such nick\r\n";
		else if (client.getNickname() == clientToKick->getNickname())
			clientMsg = "ERROR " + client.getNickname() + " " + vArguments[1] + " :You can't kick yourself\r\n";
		else
		{
			clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost KICK " + channel->getName() + " " + clientToKick->getNickname() + "\r\n";
			channel->sendToAllClients(clientMsg);
			channel->deleteClient(clientToKick);
			return;
		}
	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	if (channel && channel->getClientCount() == 0)
		server.deleteChannel(channel->getName());
}

void	Command::quit(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " QUIT :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
		clientMsg = ":" + client.getNickname() + " QUIT :Quitted \r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

		if (client.getIsLogged())
		{
			client.setIsLogged(false);
		}
		//if (client.getFd() != 1000 && (epoll_ctl(server._epollFD, EPOLL_CTL_DEL, client.getFd(), NULL) ==  -1))
		//{ chiudere fd}
	}

}

void	Command::topic(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	if(vArguments.size() == 0)
	{
		//channel->getTopic() == "" ?
		//clientMsg = "331 " + client.getNickname() + " " + vArguments[0] + " :No topic is set\r\n" :
		//clientMsg = "332 " + client.getNickname() + " #" + channel->getName() + " :" + channel->getTopic() + "\r\n";
		clientMsg = "ERROR Topic need a parameter\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		return;
	}
	Channel *channel = client.getChannel(vArguments[0]);
	if(!channel)
		clientMsg = "403 " + client.getNickname() + " " + vArguments[0] + " :No such channel\r\n";
	else if(channel->getTopicRestrict() && !channel->isOperator(client))
		return;
	else
	{
		channel->setTopic(vArguments);
		clientMsg = "332 " + client.getNickname() + " " + channel->getName() +  " :" + channel->getTopic() + "\r\n";

	}
	if (clientMsg.find("332") != std::string::npos)
		channel->sendToAllClients(clientMsg);
	else
			send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

void	handleModeOption(Client &client, std::string channelName, std::string mode, std::string extra)
{
	std::string clientMsg = "";
	Channel *channel = client.getChannel(channelName);
	Client *clientToHandle = NULL;
	if (!channel)
		return;
	if (mode[0] == '+')
	{
		for (size_t i = 1; i < mode.size(); i++)
		{
			switch (mode[i])
			{
			case 'i':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " +i\r\n";
				channel->setInviteOnly(true);
				break;
			case 't':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " +t\r\n";
				channel->setTopicRestrict(true);
				break;
			case 'k':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " +k " + extra + "\r\n";
				channel->setPassword(extra);
				std::cout << "password: " << channel->getPassword() << std::endl;
				break;
			case 'o':
				clientToHandle = channel->getClientPtr(extra);
				if (clientToHandle == NULL)
					clientMsg = "ERROR " + extra + " :user is not in this channel\r\n";
				else if (channel->isClientOnChannel(extra) && !channel->isOperator(clientToHandle->getNickname()))
				{
					clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " +o " + extra + "\r\n";
					channel->addOperatorClient(extra);
				}
				else
					clientMsg = "Error " + extra + " :user is not in this channel or is already operator\r\n";
				break;
			case 'l':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " +l " + extra + "\r\n";
				channel->setClientsMax(atoi(extra.c_str()));
				break;
			default:
				break;
			}
		}
	}
	else if (mode[0] == '-')
	{
		for (size_t i = 1; i < mode.size(); i++)
		{
			switch (mode[i])
			{
			case 'i':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " -i\r\n";
				channel->setInviteOnly(false);
				break;
			case 't':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " -t\r\n";
				channel->setTopicRestrict(false);
				break;
			case 'k':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " " + "-k\r\n";
				channel->setPassword("");
				break;
			case 'o':
				clientToHandle = channel->getClientPtr(extra);
				if (clientToHandle == NULL)
					clientMsg = "ERROR :user "  + extra +  " is not in this channel\r\n";
				else if (channel->isClientOnChannel(extra) && channel->isOperator(extra) && client.getNickname() != extra)
				{
					clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " -o " + extra + "\r\n";
					channel->deleteOperatorClient(extra);
				}
				else
					clientMsg = "ERROR :user "  + extra +  " is not operator or you can't use it on yourself\r\n";
				break;
			case 'l':
				clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost MODE " + channel->getName() + " -l\r\n";
				channel->setClientsMax(0);
				break;
			default:
				break;
			}
		}
	}
	channel->sendToAllClients(clientMsg);
}

void	Command::mode(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	/*
	i: +-i Set/remove Invite-only channel
	t: +-t Set/remove the restrictions of the TOPIC command to channel operators
	k: -+k [password] Set/remove the channel key (password)
	o: -+o [tizio] Give/take channel operator privilege
	l: +-l [number] Set/remove the user lim it to channel*/
	std::string clientMsg = "";
	Channel *channel = NULL;

	if (vArguments.size() == 1 && vArguments[1][0] == '#')
		return;
	if (vArguments.size() < 2)
		clientMsg = "461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
	else
	{
		if (vArguments[0][0] != '#')
			clientMsg = "ERROR " + client.getNickname() + " How to use MODE: /MODE #channel_name +/-option extra param\r\n";
		else
		{
			channel = client.getChannel(vArguments[0]);
			if (!channel)
				clientMsg = "403 " + client.getNickname() + " " + vArguments[0] + " :No such channel\r\n";
			else if (vArguments[1].length() > 2 && vArguments[1].length() == 2 && vArguments[1][0] != '+' && vArguments[1][0] != '-')
				clientMsg = "501" + client.getNickname() + " :Unknown MODE flag\r\n";
			else if (vArguments[1] == "+b")
				return;
			else if (!channel->isOperator(client))
				clientMsg = "482 " + client.getNickname() + " :You're not channel operator\r\n";
			else if (vArguments.size() > 2)
				handleModeOption(client, vArguments[0], vArguments[1], vArguments[2]);
			else
				handleModeOption(client, vArguments[0], vArguments[1], "");
		}
	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}


void	Command::inv(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	Client *clientToInvite = NULL;
	Channel *channel = NULL;

	if (vArguments.size() < 2)
	{
		clientMsg = "461 " + client.getNickname() + " INV :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		return	;
	}
	channel = client.getChannel(vArguments[1]);
	clientToInvite = server.getClient(vArguments[0]);

	if (!clientToInvite || !channel)
		clientMsg = "401 " + client.getNickname() + " " + vArguments[0] + " :No such nick\r\n";
	else if(channel->isClientOnChannel(clientToInvite->getNickname()))
		clientMsg = "443 " + client.getNickname() + " " + vArguments[0] + " " + channel->getName() + " :is already on channel\r\n";
	else if (channel->isClientOnChannel(client.getNickname()) == false)
		clientMsg = "442 " + client.getNickname() + " " + channel->getName() + " :You're not on that channel \r\n";
	else if (channel->isOperator(client) == false)
		clientMsg = "482 " + client.getNickname() + " " + channel->getName() + " :You're not channel operator \r\n";
	else
	{
		channel->addInvitedClient(clientToInvite);
		clientMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost INVITE " + vArguments[0] + " " + vArguments[1] + "\r\n";
		send(clientToInvite->getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		return;
	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

}

void	Command::user(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " USER :Not enough parameters\r\n";
	}
	else
		client.setUsername(vArguments[0]);
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}


#include "../includes/Command.hpp"

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
		if (!channel->getInviteOnly())
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
		}
		else
		{
			clientMsg = "473 " + client.getNickname() + " " + channelName + " :Cannot join channel, InviteOnly channel!\r\n";
			send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
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
	(void)client;
	(void)vArguments;
	std::string clientMsg = "";
	switch (vArguments.size())
	{
	case 0:
		clientMsg = "PACCIANI: " + client.getNickname() + " :test compagno di merende";
		break;
	case 1:
		clientMsg = "bot: " + client.getNickname() + " :case1";
		break;
	default:
		clientMsg = "bot: " + client.getNickname() + " :default";
		break;
	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
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
	else if (vectorSize == 1)
	{
		std::istringstream channels(vArguments[0]);
		while (std::getline(channels, channelName, ','))
		{
			//add user to channel with no password
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
			addClientToChannel(server, client, channelName, password);
		}
	}
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
	{
		clientMsg = "403 " + client.getNickname() + " " + channelName + " :No such channel\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
		std::vector<Client *> clientToMsg = channel->getLoggedClients(client);
		for (size_t i = 0; i < clientToMsg.size(); i++)
		{
			clientMsg = ":" + client.getNickname() + "! PRIVMSG " + channelName + " :" + msg + "\r\n";
			send(clientToMsg[i]->getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		}
	}
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
	if (vArguments.size() < 2)
	{
		clientMsg = "461 " + client.getNickname() + " KICK :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
	
	}
}

void	Command::quit(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	// come si disconnette?
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " QUIT :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
	
	}

	//va segnalato che un client si disconnette???
}

void	Command::topic(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	Channel *channel = client.getChannel(vArguments[0]);
	if(!channel)
		clientMsg = "403 " + client.getNickname() + " " + vArguments[0] + " :No such channel\r\n";
	if(vArguments.size() == 0)
	{
		channel->getTopic() == "" ? 
		clientMsg = "331 " + client.getNickname() + " " + vArguments[0] + " :No topic is set\r\n" :
		clientMsg = "332 " + client.getNickname() + " #" + channel->getName() + " :" + channel->getTopic() + "\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		return;
	}
	else if(channel->getTopicRestrict() && !channel->isOperator(client))
		return;// da vdere l'errore
	else
	{
		channel->setTopic(vArguments);
		clientMsg = 	"332 " + client.getNickname() + " " + channel->getName() +  " :" + channel->getTopic() + "\r\n";

	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	/*if (clientMsg.find("332") == std::string::npos)
	{
		send message to all client
	}*/
	
}

void	handleModeOption(Client &client, std::string channelName, std::string mode, std::string extra)
{
	Channel *channel = client.getChannel(channelName);
	if (!channel)
		return;
	if (mode[0] == '+')
	{
		for (size_t i = 1; i < mode.size(); i++)
		{
			switch (mode[i])
			{
			case 'i':
				channel->setInviteOnly(true);
				break;
			case 't':
				channel->setTopicRestrict(true);
				break;
			case 'k':
				channel->setPassword(extra);
				break;
			case 'o':
				//channel->AddOperator();
				break;
			case 'l':
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
				channel->setInviteOnly(false);
				break;
			case 't':
				channel->setTopicRestrict(false);
				break;
			case 'k':
				channel->setPassword("");
				break;
			case 'o':
				//channel->deleteOperator(extra);
				break;
			case 'l':
				channel->setClientsMax(0);
				break;
			default:
				break;
			}
		}
	}
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
			return;
		channel = client.getChannel(vArguments[0]);
		if (!channel)
			clientMsg = "403 " + client.getNickname() + " " + vArguments[0] + " :No such channel\r\n";
		else if (vArguments[1].length() > 2 && vArguments[1].length() == 2 && vArguments[1][0] != '+' && vArguments[1][0] != '-')
			clientMsg = "501" + client.getNickname() + " :Unknown MODE flag\r\n";
		else if (channel->isOperator(client))
			clientMsg = "482 " + client.getNickname() + " :You're not channel operator\r\n";
		else if (vArguments.size() > 2)
			handleModeOption(client, vArguments[0], vArguments[1], vArguments[2]);
		else
			handleModeOption(client, vArguments[0], vArguments[1], "");
	}
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}


void	Command::inv(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " INV :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
	
	}
}

void	Command::user(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	(void)server;
	std::string clientMsg = "";
	//da implementare se gia registrato
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " USER :Not enough parameters\r\n";
	}
	else
		client.setUsername(vArguments[0]);
	send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

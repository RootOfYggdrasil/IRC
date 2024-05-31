
#include "../includes/Command.hpp"

Command::Command(void) {}
Command::~Command(void) {}

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
	size_t	vectorSize = !vArguments.size();

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
		}	
	}	
	else if (vectorSize == 2)
	{
		std::istringstream channels(vArguments[0]);
		std::istringstream passwords(vArguments[1]);
		while (std::getline(channels, channelName, ','))
		{
			//add user to channel with  password
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
		// send message to client
	}	
}

void	Command::nick(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " NICK :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
	
	}
}

void	Command::kick(Server &server, Client &client, std::vector<std::string> &vArguments)
{
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
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " QUIT :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
	
	}
}

void	Command::topic(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " TOPIC :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
	
	}
}


void	Command::mode(Server &server, Client &client, std::vector<std::string> &vArguments)
{
	std::string clientMsg = "";
	if (vArguments.size() < 1)
	{
		clientMsg = "461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
		send(client.getFd(), clientMsg.c_str(), clientMsg.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	else
	{
	
	}
}


void	Command::inv(Server &server, Client &client, std::vector<std::string> &vArguments)
{
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



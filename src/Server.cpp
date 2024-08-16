/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:43 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/16 19:54:24 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _socket(0) {
	_password = "1234";
	// set the fd table at 0
	FD_ZERO(&this->_rfds);
	FD_ZERO(&this->_rfds_error);
	FD_ZERO(&this->_rfds_read);
	FD_ZERO(&this->_rfds_write);
	this->_status_server = SUCCESS;
}

Server::Server(char *psw, int port) : _password(psw), _socket(port)
{
	FD_ZERO(&this->_rfds);
	FD_ZERO(&this->_rfds_error);
	FD_ZERO(&this->_rfds_read);
	FD_ZERO(&this->_rfds_write);
	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flux binaire n
	this->addBot();
	this->_status_server = SUCCESS;
}

void	Server::addBot(void) // cause des problem
{
	Client	*buffer;
	int		botFd;

	botFd = -666;
	buffer = new (std::nothrow) Bot(botFd);
	if (buffer)
	{
		try
		{
			this->_clientList.insert(std::pair<int, Client *>(botFd, buffer));
		}
		catch(const std::exception&)
		{
			delete buffer;
		}
	}
}

Server::~Server(void)
{
	while (this->_clientList.begin() != this->_clientList.end())
		this->deletClient(this->_clientList.begin()->first);
	while (this->_channels.begin() != this->_channels.end())
	{
		delete (this->_channels.begin()->second);
		this->_channels.erase(this->_channels.begin());
	}
}

fd_set	Server::getFdSet(void) const
{
	return (this->_rfds);
}

Client	*Server::getClient(int const fd)
{
	Client	*buffer;

	buffer = NULL;
	if (this->_clientList.find(fd) != this->_clientList.end())
		buffer = this->_clientList.find(fd)->second;
	return (buffer);
}

void	Server::addClient(int const fd)
{
	Client	*buffer;

	if (this->_status_server == SUCCESS
		&& this->_clientList.find(fd) == this->_clientList.end())
	{
		buffer = new (std::nothrow) Client(fd);
		if (buffer)
		{
			// add the new client fd to the table
			try
			{
				this->_clientList.insert(std::pair<int, Client *>(fd, buffer));
				FD_SET(fd, &this->_rfds);
			}
			catch(const std::exception& )
			{
				close(fd);
				delete buffer;
			}
		}
		else
			close(fd);
	}
}

void	Server::searchClient(void)
{
	int	new_client;
	
	if (this->_status_server == SUCCESS)
	{
		new_client = accept(this->_socket.getSocketFd(), NULL, 0);
		if (new_client != -1)
		{
			this->addClient(new_client);
		}
	}
}

void	Server::deletClient(int const fd)
{
	std::map<std::string, Channel *>::iterator	it_channel;
	std::map<std::string, Channel *>::iterator	it_old_channel;

	if ( this->_status_server == SUCCESS
		&& this->_clientList.find(fd) != this->_clientList.end())
	{
		it_channel = this->_channels.begin();
		while (it_channel != this->_channels.end())
		{
			it_old_channel = it_channel;
			it_channel++;
			it_old_channel->second->part(this->_clientList[fd], "", true);
			it_old_channel->second->eraseInviteLst(this->_clientList[fd]);
			closeChannel(it_old_channel->first, this->_channels);
		}
		// remove the client from the table
		delete this->_clientList[fd];
		if (fd >= 0)
		{
			FD_CLR(fd, &this->_rfds);
			close(fd);
		}
		this->_clientList.erase(fd);
	}
}

int		Server::getStatus(void) const
{
	return (this->_status_server);
}

void	Server::clientRecvMessage(int const client_fd, Client& client)
{
	int		ret_recv;
	char	buffer[SIZE_MESSAGE_BUFFER + 1] = {0};

	ret_recv = SIZE_MESSAGE_BUFFER;
	while (ret_recv == SIZE_MESSAGE_BUFFER)
	{
		ret_recv = recv(client_fd, buffer, SIZE_MESSAGE_BUFFER, MSG_DONTWAIT);
		// return the message only if the connection is still open (otherwise it crashes)
		if (ret_recv > 0)
		{
			//send(client_fd, buffer, std::strlen(buffer), 0);
			client.addCommandBuffer(buffer);
		}
		else if (ret_recv == 0)
		{
			//FD_CLR(client_fd, &this->_rfds_read);
			client.terminateConnection();
		}
		std::memset(buffer, 0, SIZE_MESSAGE_BUFFER);
	}
}

void	Server::clientSendMessage(int const client_fd, Client& client)
{
	std::string	buffer;

	buffer = client.getRPL();
	std::cout << "Sending to client: " << buffer << std::endl;
	send(client_fd, buffer.c_str(), buffer.length(), 0);
}

void	Server::clientRecv(void)
{
	std::map<int, Client*>::iterator	it;
	
	if (this->_clientList.size() > 0)
	{
		it = this->_clientList.begin();
		while (it != this->_clientList.end())
		{
			// to check if the requested fd is in the list
			if (FD_ISSET(it->first, &this->_rfds_read))
				this->clientRecvMessage(it->first, *it->second);
			it++;
		}
	}
}

void	Server::sendBot(Client *bot)
{
	Bot			*cbot;
	std::string	rplBuffer;
	size_t		len;

	cbot = dynamic_cast<Bot *>(bot);
	rplBuffer = bot->getRPL();
	if (bot)
	{
		while (rplBuffer.empty() == false)
		{
			len = rplBuffer.find_first_of('\n');
			cbot->RPL(rplBuffer.substr(0, len));
			rplBuffer.erase(0, ((len <= rplBuffer.length())? len + 1 :len));
		}
	}
}

void	Server::clientSend(void)
{
	std::map<int, Client*>::iterator	it;
	
	if (this->_clientList.size() > 0)
	{
		it = this->_clientList.begin();
		while (it != this->_clientList.end())
		{
			// to check if the requested fd is in the list
			if (it->second->getRPLBuffer().empty() == false)
			{
				if (FD_ISSET(it->first, &this->_rfds_write))
					this->clientSendMessage(it->first, *it->second);
				else if (it->second->getBot())
					this->sendBot(it->second);
			}
			it++;
		}
	}
}

void	Server::eraseClient(void)
{
	std::map<int, Client*>::iterator	it;
	std::map<int, Client*>::iterator	itNext;

	it = this->_clientList.begin();
	while (it != this->_clientList.end())
	{
		itNext = it;
		itNext++;
		if (it->second->getStatusClient() == CS_TERMINATED)
		{
			this->deletClient(it->first);
		}
		it = itNext;
	}
}

void	Server::useSelect(void)
{
	std::map<int, Client*>::iterator	buffer;
	struct timeval						tv;
	
	if (this->_clientList.size() > 0)
	{
		buffer = this->_clientList.end();
		buffer--;
		if (buffer->first >= 0)
		{
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			this->_rfds_read = this->_rfds;
			this->_rfds_write = this->_rfds;
			this->_rfds_error = this->_rfds;
			// find out who has communicated
			select(buffer->first + 1, &this->_rfds_read,
				&this->_rfds_write, &this->_rfds_error, &tv);
		}
	}
}

void	Server::parseInput(void) {
	std::map<int, Client*>::iterator	it;
	std::map<int, Client*>::iterator	ite;
	Client								*client;

	it = this->_clientList.begin();
	ite = this->_clientList.end();
	while (it != ite){
		std::string tkt; // a enveler ca
		client = it->second;
		while (client->getCommandValible()){
			tkt = client->getCommand();		
			this->parse(tkt, *client);
		}
		it++;
	}
}

void	Server::userPing(void)
{
	time_t								ctime;
	std::map<int, Client*>::iterator	it;
	std::map<int, Client*>::iterator	ite;
	Client								*client;

	it = this->_clientList.begin();
	ite = this->_clientList.end();
	ctime = std::time(NULL);
	while (it != ite)
	{
		client = it->second;
		if (client->getBot() == false
			&& client->lastPingTime(ctime) > MAX_TIME_PING)
		{
			if (client->getSendPing())
				client->terminateConnection();
			else
				client->addRPLBuffer("PING :coucou\n");
			client->setLastPing(ctime);
			client->setSendPing(true);
		}
		it++;
	}
}

void	Server::execut(void)
{
	this->useSelect();
	this->searchClient();
	this->clientRecv();
	this->parseInput();
	this->userPing();
	this->clientSend();
	this->eraseClient();
}

static enum type guessType(std::stringstream &input)
{
	std::string cmd;
	input >> cmd;
	for (unsigned int i = 0 ; cmd[i] != 0 ; i++) {
		cmd[i] = std::toupper(cmd[i]);
	}
	std::cout << cmd << std::endl;
	if (!cmd.compare("PASS")|| !cmd.compare("NICK")
		|| !cmd.compare("USER") || !cmd.compare("QUIT")
		|| !cmd.compare("CAP"))
		return (CONNEXION);
	else if (!cmd.compare("JOIN") || !cmd.compare(0, 5, "PART")
		|| !cmd.compare("TOPIC") || !cmd.compare(0, 6, "NAMES")
		|| !cmd.compare("LIST") || !cmd.compare(0, 7, "INVITE")
		|| !cmd.compare("KICK") || !cmd.compare(0, 5, "MODE")
		|| !cmd.compare("PRIVMSG") || !cmd.compare(0, 5, "PING")
		|| !cmd.compare("PONG"))
		return (CHANNEL);
	else if (cmd.empty())
		return (EMPTY);
	return (ERR);
}

// Maybe I'll have to separate the exception handler from the parsing
void	Server::parse(std::string cmd, Client &c)
{
	try
	{
		std::stringstream preparsed_input(cmd);
		enum type t = guessType(preparsed_input);
		if (t == ERR)
			throw (IRCError(ERR_UNKNOWNCOMMAND, c.getNickName(), cmd));
		else if (t == CONNEXION)
			ConnexionCommand(cmd, _password, this->_clientList, c).execute(c);
		else if (t == CHANNEL)
			ChannelCommand(cmd).execute(&c, this->_channels, this->_clientList);
	}
	catch (IRCError &e)
	{
		std::cout << e.what() << std::endl; // to remove, only used for tests.
		c.addRPLBuffer(e.getReply());
		if (e.getErr() == ERR_PASSWDMISMATCH)
			c.terminateConnection();
	}
	catch (std::exception &e)
	{
		std::cout << "LOG :" << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unhandled exception" << std::endl;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:43 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 22:45:03 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::~Server(void)
{
	while (this->_clientList.begin() != this->_clientList.end())
		this->deleteClient(this->_clientList.begin()->first, true);
	while (this->_channels.begin() != this->_channels.end())
	{
		delete (this->_channels.begin()->second);
		this->_channels.erase(this->_channels.begin());
	}
}

Server::Server(char *psw, int port) : _password(psw), _socket(port)
{
	FD_ZERO(&this->_rfds);
	FD_ZERO(&this->_rfds_error);
	FD_ZERO(&this->_rfds_read);
	FD_ZERO(&this->_rfds_write);
	this->addBot();
	this->_status = SUCCESS;
}

void	Server::addBot(void)
{
	Client	*buffer;
	int		botFd;

	botFd = -666;
	buffer = new (std::nothrow) Bot();
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

void	Server::addClient(int const fd)
{
	Client	*buffer;

	if (this->_status == SUCCESS
		&& this->_clientList.find(fd) == this->_clientList.end())
	{
		buffer = new (std::nothrow) Client();
		if (buffer)
		{
			try
			{
				this->_clientList.insert(std::pair<int, Client *>(fd, buffer));
				FD_SET(fd, &this->_rfds);
			}
			catch(const std::exception &e)
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
	
	if (this->_status == SUCCESS)
	{
		new_client = accept(this->_socket.getFd(), NULL, NULL);
		if (new_client != -1)
			this->addClient(new_client);
	}
}

void	Server::deleteClient(int const fd, bool serverTerminate)
{
	std::map<std::string, Channel *>::iterator	it_channel;
	std::map<std::string, Channel *>::iterator	it_old_channel;

	if (this->_status == SUCCESS
		&& this->_clientList.find(fd) != this->_clientList.end())
	{
		it_channel = this->_channels.begin();
		while (!serverTerminate && it_channel != this->_channels.end())
		{
			it_old_channel = it_channel;
			it_channel++;
			it_old_channel->second->part(this->_clientList[fd], "");
			it_old_channel->second->eraseInviteLst(this->_clientList[fd]);
			closeChannel(it_old_channel->first, this->_channels);
		}
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
	return (this->_status);
}

void	Server::clientRecvMessage(int const client_fd, Client& client)
{
	int		ret_recv;
	char	buffer[SIZE_MESSAGE_BUFFER + 1] = {0};

	ret_recv = SIZE_MESSAGE_BUFFER;
	while (ret_recv == SIZE_MESSAGE_BUFFER)
	{
		ret_recv = recv(client_fd, buffer, SIZE_MESSAGE_BUFFER, MSG_DONTWAIT);
		if (ret_recv > 0)
			client.addCommandBuffer(buffer);
		else if (ret_recv == 0)
			client.terminateConnection();
		std::memset(buffer, 0, SIZE_MESSAGE_BUFFER);
	}
}

void	Server::clientSendMessage(int const client_fd, Client& client)
{
	std::string	buffer;

	buffer = client.getRPL();
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
	if (cbot)
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
		if (it->second->getStatus() == CS_TERMINATED)
		{
			this->deleteClient(it->first, false);
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
		client = it->second;
		while (client->getCommandValided()){
			this->parse(client->getCommand(), *client);
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

void	Server::execute(void)
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
	if (!cmd.compare("PASS")|| !cmd.compare("NICK")
		|| !cmd.compare("USER") || !cmd.compare("QUIT")
		|| !cmd.compare("CAP") || !cmd.compare("PING")
		|| !cmd.compare("PONG"))
		return (CONNEXION);
	else if (!cmd.compare("JOIN") || !cmd.compare("PART")
		|| !cmd.compare("TOPIC") || !cmd.compare("NAMES")
		|| !cmd.compare("LIST") || !cmd.compare("INVITE")
		|| !cmd.compare("KICK") || !cmd.compare("MODE")
		|| !cmd.compare("PRIVMSG") || !cmd.compare("WHO")
		|| !cmd.compare("WHOIS"))
		return (CHANNEL);
	else if (cmd.empty())
		return (EMPTY);
	return (ERR);
}

void	Server::parse(std::string cmd, Client &c)
{
	try
	{
		if (cmd.length() > MESSAGES_LIMIT)
			throw (IRCError(ERR_INPUTTOOLONG, c.getNickName()));
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
		c.addRPLBuffer(e.getReply());
		if (e.getErr() == ERR_PASSWDMISMATCH)
			c.terminateConnection();
	}
	catch (std::exception &e)
	{
		std::cout << "LOG :" << e.what() << std::endl;
		throw (e);
	}
	catch (...)
	{
		std::cout << "Unhandled exception" << std::endl;
	}
}

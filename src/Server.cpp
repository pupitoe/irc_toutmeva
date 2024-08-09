/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:43 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/09 09:53:02 by ggiboury         ###   ########.fr       */
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
	this->_status_server = SUCCESS;
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
			FD_SET(fd, &this->_rfds);
			this->_clientList.insert(std::pair<int, Client *>(fd, buffer));
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
			std::cout << "client fd: " << new_client << std::endl;
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
		FD_CLR(fd, &this->_rfds);
		delete this->_clientList[fd];
		close(fd);
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

void	Server::clientSend(void)
{
	std::map<int, Client*>::iterator	it;
	
	if (this->_clientList.size() > 0)
	{
		it = this->_clientList.begin();
		while (it != this->_clientList.end())
		{
			// to check if the requested fd is in the list
			if (it->second->getRPLBuffer().empty() == false &&
				FD_ISSET(it->first, &this->_rfds_write))
				this->clientSendMessage(it->first, *it->second);
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
			std::cout << "client: " << it->first << "; closed" << std::endl;
			this->deletClient(it->first);
		}
		it = itNext;
	}
}

void	Server::useSelect(void) {
	std::map<int, Client*>::iterator	buffer;
	struct timeval						tv;
	
	if (this->_clientList.size() > 0)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		this->_rfds_read = this->_rfds;
		this->_rfds_write = this->_rfds;
		this->_rfds_error = this->_rfds;
		buffer = this->_clientList.end();
		buffer--;
		// find out who has communicated
		select(buffer->first + 1, &this->_rfds_read,
			&this->_rfds_write, &this->_rfds_error, &tv);
	}
}

void	Server::parseInput(void) {
	std::map<int, Client*>::iterator	it;
	std::map<int, Client*>::iterator	ite;
	Client								*client;

	it = this->_clientList.begin();
	ite = this->_clientList.end();
	while (it != ite){
		std::string tkt;
		client = it->second;
		while (client->getCommandValible()){
			tkt = client->getCommand();		
			std::cout << "!cmd: " << tkt << std::endl;
			this->parse(tkt, *client);
			std::cout << "bijour" << std::endl;
		}
		it++;
	}
}

void	Server::execut(void) {
	this->useSelect();
	this->searchClient();
	this->clientRecv();
	this->parseInput();
	this->eraseClient();
	this->clientSend();
}

static enum type guessType(std::string msg) {
	if (!msg.compare(0, 5, "PASS ", 5)|| !msg.compare(0, 5, "NICK ", 5)
		|| !msg.compare(0, 5, "USER ", 5) || !msg.compare(0, 4, "CAP ", 4))
		return (CONNEXION);
	else if (!msg.compare(0, 5, "JOIN ", 5) || !msg.compare(0, 5, "PART ", 5)
		|| !msg.compare(0, 6, "TOPIC ", 6) || !msg.compare(0, 6, "NAMES ", 6)
		|| !msg.compare(0, 5, "LIST ", 5) || !msg.compare(0, 7, "INVITE ", 7)
		|| !msg.compare(0, 5, "KICK ", 5) || !msg.compare(0, 5, "MODE ", 5)
		|| !msg.compare(0, 8, "PRIVMSG ", 8))
		return (CHANNEL);
	else if (msg.empty())
		return (EMPTY);
	return (ERR);
}

// Maybe I'll have to separate the exception handler from the parsing
// elever les new
void	Server::parse(std::string cmd, Client &c) {
	try {
		Command	*rqst = NULL;
		enum type t = guessType(cmd);
		std::cout << t << std::endl;
		if (t == ERR)
			throw (Command::UnrecognizedType());
		else if (t == CONNEXION)
			rqst = new ConnexionCommand(cmd, _password);
		else if (t == CHANNEL)
			rqst = new ChannelCommand(cmd);
		if (rqst)
		{
			std::cout << *rqst << std::endl;
			this->executeRequests(c, rqst);
		}
	}
	catch (IRCError &e) {
		std::cout << e.what() << std::endl; // to remove, only used for tests.
		c.addRPLBuffer(e.getError());
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unhandled exception" << std::endl;
	}
}

void	Server::executeRequests(Client& client, Command *rqst) {
	//Creer une fonction foreach pour les clients ?
	if (rqst->getType() == CHANNEL)
		rqst->execute(&client, this->_channels, this->_clientList);
	else
		rqst->execute(client);
	delete (rqst);
}

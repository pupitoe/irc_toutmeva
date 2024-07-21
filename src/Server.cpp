/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:43 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/21 18:22:20 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _socket(0) {
	_password = "1234";
	// set the fd table at 0
	FD_ZERO(&this->_rfds);
	this->_status_server = SUCCESS;
}

Server::Server(char *psw, int port) : _password(psw), _socket(port)
{
	FD_ZERO(&this->_rfds);
	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flux binaire n
	this->_status_server = SUCCESS;
}

Server::~Server(void)
{
	while (this->_clientList.begin() != this->_clientList.end())
		this->deletClient(this->_clientList.begin()->first);
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
	if ( this->_status_server == SUCCESS
		&& this->_clientList.find(fd) != this->_clientList.end())
	{
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
			client.terminateConnection();
		std::memset(buffer, 0, SIZE_MESSAGE_BUFFER);
	}
	std::string tkt;
	while (client.getCommandValible()){
		tkt = client.getCommand();		
		std::cout << "!cmd: " << tkt << std::endl;
		this->parse(tkt, client);
	}
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

void	Server::useSelect(void)
{
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
	this->executeRequests();
	this->eraseClient();
}

static enum type guessType(std::string msg) {
	if (!msg.compare(0, 5, "PASS ", 5)|| !msg.compare(0, 5, "NICK ", 5)
		|| !msg.compare(0, 5, "USER ", 5) || !msg.compare(0, 4, "CAP ", 4))
		return (CONNEXION);
	else if (!msg.compare(0, 5, "JOIN ", 5) || !msg.compare(0, 5, "PART ", 5)
		|| !msg.compare(0, 5, "TOPIC ", 6) || !msg.compare(0, 5, "NAMES  ", 6)
		|| !msg.compare(0, 5, "LIST ", 5) || !msg.compare(0, 5, "INVITE ", 7)
		|| !msg.compare(0, 5, "KICK ", 5))
		return (CHANNEL);
	else if (msg.empty())
		return (EMPTY);
	return (ERR);
}

// Maybe I'll vhave to separate the exception handler from the parsing
void	Server::parse(std::string cmd, Client &c) {
	try{
		Command	*rqst = NULL;
		enum type t = guessType(cmd);
		std::cout << t << std::endl;
		if (t == ERR)
			throw (Command::UnrecognizedType());
		else if (t == CONNEXION){
			rqst = new ConnexionCommand(cmd);
		}
		else if (t == CHANNEL){
			rqst = new ChannelCommand(cmd);
		}
		std::cout << *rqst << std::endl;
		c.addRequest(rqst);
	}
	catch (IRCError::NeedMoreParams &e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
	catch (...){
		std::cout << "Unhandled exception" << std::endl;
	}
}

void	Server::executeRequests(void) {
	std::map<int, Client*>::iterator	it;
	std::map<int, Client*>::iterator	ite;
	Client								*client;
	Command								*rqst;
	
	it = this->_clientList.begin();
	ite = this->_clientList.end();
	//Creer une fonction foreach pour les clients ?
	while (it != ite) {
		client = it->second;
		while (client->hasRequest()) {
			rqst = client->nextRequest();
			rqst->execute(this);
			if (true)
				delete (rqst);
		}
		it++;
	}
}
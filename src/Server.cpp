/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:43 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/30 22:01:02 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server(void)
{
	_port = 6969;
	_password = "1234";
	// set the fd table at 0
	FD_ZERO(&this->_rfds);
	this->_status_server = FAIL;
	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flux binaire n
	this->_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket_fd != -1 && ft_setsoket(this->_socket_fd) == SUCCESS
			&& ft_socket_bind(this->_socket_fd, _port) == SUCCESS)
		this->_status_server = SUCCESS;
}

/*
* List of available port can be found on wikipedia.
* We can't use values under 1024 because rights.
* */ 
bool	port_is_valid(int p) {
	return (p == 194 || (p >= 6665 && p <= 6669));
}

Server::Server(char *psw, int port) : _password(psw), _port(port)
{
	FD_ZERO(&this->_rfds);
	this->_status_server = FAIL;
	if (!port_is_valid(_port)){
		std::cerr << "Port invalid" << std::endl;
		return ;
	}
	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flux binaire n
	this->_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket_fd != -1 && ft_setsoket(this->_socket_fd) == SUCCESS
			&& ft_socket_bind(this->_socket_fd, _port) == SUCCESS)
		this->_status_server = SUCCESS;
}

Server::~Server(void)
{
	while (this->_clientList.begin() != this->_clientList.end())
		this->deletClient(this->_clientList.begin()->first);
	if (this->_socket_fd != -1)
		close(_socket_fd);
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
		new_client = accept(this->_socket_fd, NULL, 0);
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
	while (client.getCommandValible())
		std::cout << "cmd: " << client.getCommand() << std::endl;
}

void	Server::clientRecv(void)
{
	std::map<int, Client*>::iterator	it;
	fd_set								buffer_rfds;
	struct timeval						tv;
	
	if (this->_clientList.size() > 0)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		buffer_rfds = this->_rfds;
		it = this->_clientList.end();
		it--;
		// find out who has communicated
		if (select(it->first + 1, &buffer_rfds, NULL, NULL, &tv) > 0)
		{
			it = this->_clientList.begin();
			while (it != this->_clientList.end())
			{
				// to check if the requested fd is in the list
				if (FD_ISSET(it->first, &buffer_rfds))
					this->clientRecvMessage(it->first, *it->second);
				it++;
			}
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

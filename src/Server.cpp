/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:43 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 19:13:58 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server(void)
{
	// set the fd table at 0
	FD_ZERO(&this->_rfds);
	this->_status_server = FAIL;
	// to create a communication point (AF_INET is the IPV4 protocol)
	// SOCK_STREAM creates a binary flue n
	this->_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket_fd != -1 && ft_setsoket(this->_socket_fd) == SUCCESS
			&& ft_socket_bind(this->_socket_fd) == SUCCESS)
		this->_status_server = SUCCESS;
}

Server::~Server(void)
{
	while (this->_clientList.begin() != this->_clientList.end())
	{
		close(this->_clientList.begin()->first);
		this->_clientList.erase(this->_clientList.begin());
	}
	if (this->_socket_fd != -1)
		close(_socket_fd);
}

fd_set	Server::getFdSet(void) const
{
	return (this->_rfds);
}

Client&	Server::getClient(int const fd)
{
	return (this->_clientList[fd]);
}

void	Server::addClient(int const fd)
{
	if (this->_status_server == SUCCESS
		&& this->_clientList.find(fd) == this->_clientList.end())
	{
		// add the new client fd to the table
		FD_SET(fd, &this->_rfds);
		this->_clientList.insert(std::pair<int, Client>(fd, Client()));
	}
}

void	Server::searchClient(void)
{
	int	new_clien;
	
	if (this->_status_server == SUCCESS)
	{
		new_clien = accept(this->_socket_fd, NULL, 0);
		if (new_clien != -1)
		{
			this->addClient(new_clien);
			std::cout << "client fd: " << new_clien << std::endl;
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
		close(fd);
		this->_clientList.erase(fd);
	}
}

int		Server::getStatus(void) const
{
	return (this->_status_server);
}

void	Server::clientRecvMessage(int const client_fd, Client& client_content)
{
	int		ret_recv;
	char	buffer[1000] = {0};

	ret_recv = recv(client_fd, buffer, 1000, MSG_DONTWAIT);
	// return the message only if the connection is still open (otherwise it crashes)
	if (ret_recv > 0)
		send(client_fd, buffer, 1000, 0);
	else
		client_content.changeTerminate();
	std::cout << buffer;
}

void	Server::clientRecv(void)
{
	std::map<int, Client>::iterator	it;
	fd_set							buffer_rfds;
	struct timeval					tv;
	
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
					this->clientRecvMessage(it->first, it->second);
				it++;
			}
		}
	}
}

void	Server::eraseClient(void)
{
	std::map<int, Client>::iterator	it;
	std::map<int, Client>::iterator	itNext;

	it = this->_clientList.begin();
	while (it != this->_clientList.end())
	{
		itNext = it;
		itNext++;
		if (it->second.getTerminate() == true)
		{
			std::cout << "client: " << it->first << "; closed" << std::endl;
			this->deletClient(it->first);
		}
		it = itNext;
	}
	
}

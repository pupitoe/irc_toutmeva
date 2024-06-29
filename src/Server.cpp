/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:43 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 15:34:31 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server(void)
{
	FD_ZERO(&this->_rfds);
}

Server::~Server(void)
{
	
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
	if (this->_clientList.find(fd) == this->_clientList.end())
	{
		FD_SET(fd, &this->_rfds);
		this->_clientList.insert(std::pair<int, Client>(fd, Client()));
	}
}

void	Server::deletClient(int const fd)
{
	if (this->_clientList.find(fd) != this->_clientList.end())
	{
		FD_CLR(fd, &this->_rfds);
		close(fd);
		this->_clientList.erase(fd);
	}
}
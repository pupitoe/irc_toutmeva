/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:37:27 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/30 17:47:52 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>
# include <iostream>

Client::Client(int const client_fd): _client_fd(client_fd)
{
	this->_status_connection = CS_NOTHING;
	std::cout << "client created: " << client_fd << std::endl;
	(void)this->_client_fd;
}

Client::~Client(void)
{
}

int	Client::getStatusClient(void) const
{
	return (this->_status_connection);
}

void	Client::terminateConnection(void)
{
	this->_status_connection = CS_TERMINATED;
}

void	Client::setNickName(std::string const& str)
{
	this->_nickName = str;
}

void	Client::setUserName(std::string const& str)
{
	this->_userName = str;
}

void	Client::setHostName(std::string const& str)
{
	this->_hostName = str;
}

void	Client::setUserFullName(std::string const& str)
{
	this->_userNameFull = str;
}

void	Client::setServerName(std::string const& str)
{
	this->_serverName = str;
}

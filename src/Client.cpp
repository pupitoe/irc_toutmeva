/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:37:27 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 20:07:03 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client(void)
{
	this->_terminate_connection = 0;
}

Client::~Client(void)
{
}

bool	Client::getTerminate(void) const
{
	return (this->_terminate_connection);
}
void	Client::changeTerminate(void)
{
	this->_terminate_connection = !this->_terminate_connection;
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

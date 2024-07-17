/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:37:27 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/17 16:21:59 by ggiboury         ###   ########.fr       */
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
	std::vector<Command*>::iterator it = this->requests.begin();
	std::vector<Command*>::iterator ite = this->requests.end();
	while (it != ite){
		delete (*it);
		it++;
	}
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

std::string const&	Client::getCommandBuffer(void) const
{
	return (this->_bufferCommand);
}

std::string	Client::getCommand(void)
{
	std::string	cmd;

	if (this->getCommandValible())
	{
		cmd = this->_bufferCommand.substr(0,
			this->_bufferCommand.find_first_of('\n'));
		this->_bufferCommand = this->_bufferCommand.c_str()
			+ this->_bufferCommand.find_first_of('\n') + 1;
	}
	else
	{
		cmd = this->_bufferCommand;
		this->_bufferCommand = "";
	}
	return (cmd);
}

bool	Client::getCommandValible(void)
{
	return (this->_bufferCommand.find_first_of('\n')
		< this->_bufferCommand.length());
}

void	Client::addCommandBuffer(char const *cmd)
{
	this->_bufferCommand += cmd;
}

void	Client::addRequest(Command c){
	Command *new_com = new Command(c);
	this->requests.push_back(new_com);
}
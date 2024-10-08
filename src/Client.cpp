/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:37:27 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 21:53:19 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client(void)
{
	this->_status_connection = CS_NOTHING;
	this->_lastPing = std::time(NULL);
	this->_nickName = "a";
	this->_sendPing = false;
	this->_bot = false;
}

Client::~Client(void)
{
}

int	Client::getStatus(void) const 
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

void	Client::setRealName(std::string const& str)
{
	this->_userRealName = str;
}

void	Client::setServerName(std::string const& str)
{
	this->_serverName = str;
}

void	Client::addStatus(enum CStatus status)
{
	this->_status_connection |= status;
}

void	Client::removeStatus(enum CStatus status)
{
	if (this->_status_connection & status)
		this->_status_connection = this->_status_connection ^ status;
}

std::string const&	Client::getCommandBuffer(void) const
{
	return (this->_bufferCommand);
}

void	Client::setLastPing(time_t ctime)
{
	this->_lastPing = ctime;
}

void	Client::setSendPing(bool set)
{
	this->_sendPing = set;
}

bool	Client::getSendPing(void) const
{
	return (this->_sendPing);
}

std::string	Client::getCommand(void)
{
	std::string	cmd;
	size_t		len;

	len = 0;
	if (this->getCommandValided())
	{
		cmd = this->_bufferCommand.substr(0,
			this->_bufferCommand.find_first_of('\n'));
		this->_bufferCommand = this->_bufferCommand.c_str()
			+ this->_bufferCommand.find_first_of('\n') + 1;
		len = cmd.length();
		if (len >= 1 && cmd[len - 1] == '\r')
			cmd.erase(len - 1, 1);
	}
	else
	{
		cmd = this->_bufferCommand;
		this->_bufferCommand = "";
	}
	return (cmd);
}

bool	Client::getCommandValided(void)
{
	return (this->_bufferCommand.find_first_of('\n')
		< this->_bufferCommand.length());
}

void	Client::addCommandBuffer(char const *cmd)
{
	this->_bufferCommand += cmd;
}

void	Client::addCommandBuffer(std::string const& cmd)
{
	this->_bufferCommand += cmd;
}

void	Client::addRPLBuffer(std::string const& rpl)
{
	this->_bufferRPL += rpl;
}

std::string	Client::getRPL(void)
{
	std::string buffer;

	buffer = this->_bufferRPL;
	this->_bufferRPL.clear();
	return (buffer);
}

std::string const&	Client::getRPLBuffer(void) const
{
	return (this->_bufferRPL);
}

std::string const&	Client::getNickName(void) const
{
	return (this->_nickName);
}

Client	*getClientMap(std::string const& nickName,
	std::map<int, Client*>clientsLst)
{
	std::map<int, Client *>::iterator	it;
	std::map<int, Client *>::iterator	itend;
	Client								*buffer;

	buffer = NULL;
	it = clientsLst.begin();
	itend = clientsLst.end();
	while (it != itend && it->second->getNickName() != nickName)
		it++;
	if (it != itend)
		buffer = it->second;
	return (buffer);
}

time_t	Client::lastPingTime(time_t ctime) const
{
	return (ctime - this->_lastPing);
}

bool	Client::getBot(void) const
{
	return (this->_bot);
}

std::string			Client::getInfo(void) const
{
	return (this->_nickName + "!~" + this->_userName + "@" + this->_hostName);
}

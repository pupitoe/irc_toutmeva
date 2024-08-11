/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:41 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 01:19:09 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Bot.hpp>
#include <BotCommand.hpp>

Bot::Bot(int const client_fd): Client(client_fd)
{
	this->_nickName = BOT_NAME;
	this->_bot = true;
	this->_status_connection = CS_CONNECTED;
	this->_hostName = "irctoutmevas";
	this->_userName = BOT_NAME;
	this->_userNameFull = BOT_NAME;
	this->_serverName = "irctoutmevas";
}

Bot::~Bot(void)
{
}

void	Bot::RPL(std::string const str)
{
	try
	{
		BotCommand	cmd(str);

		cmd.execute(*this);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	std::cout << "RPL BOT STR: " << str << std::endl;
}

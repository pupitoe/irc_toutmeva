/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:41 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/13 21:17:31 by tlassere         ###   ########.fr       */
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
	BotCommand	cmd(str, this);

	cmd.execute();
	std::cout << "RPL BOT STR: " << str << std::endl;
}

std::string	Bot::game(std::string const& params)
{
	if (this->_morfiGames.size() > MAX_MORFI_GAMES)
		return ("trop de joueur joue");
	(void)params;
	return ("c bon tkt frefrot");
}

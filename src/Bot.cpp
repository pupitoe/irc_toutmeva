/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:41 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/14 22:59:59 by tlassere         ###   ########.fr       */
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
	while (this->_morfiGames.begin() != this->_morfiGames.end())
	{
		delete this->_morfiGames.begin()->second;
		this->_morfiGames.erase(this->_morfiGames.begin());
	}
	
}

void	Bot::RPL(std::string const str)
{
	BotCommand	cmd(str, this);

	cmd.execute();
	std::cout << "RPL BOT STR: " << str << std::endl;
}

std::map<std::string, Morfi*>&	Bot::getMorfi(void)
{
	return (this->_morfiGames);
}

int	Bot::getMorfiStat(std::string const& target)
{
	std::map<std::string, Morfi*>::iterator	game;

	game = this->_morfiGames.find(target);
	if (game == this->_morfiGames.end())
		return (MO_NOT_CREAT);
	return (game->second->getRound());
}

bool	Bot::creatGame(std::string const& target, std::string const gameName)
{
	Morfi	*buffer;
	bool	status;

	status = false;
	buffer = NULL;
	if (this->_morfiGames.find(gameName) == this->_morfiGames.end())
		buffer = new (std::nothrow) Morfi(this->_nickName, target);
	if (buffer)
	{
		try
		{
			this->_morfiGames.insert(std::pair<std::string, Morfi*>(gameName,
				buffer));
			status = true;
		}
		catch(const std::exception& e)
		{
			delete buffer;
		}
	}
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:41 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 21:06:50 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Bot.hpp>
#include <BotCommand.hpp>

Bot::Bot(void): Client()
{
	this->_nickName = BOT_NAME;
	this->_bot = true;
	this->_status_connection = CS_CONNECTED;
	this->_hostName = "fabio";
	this->_userName = "flo";
	this->_userRealName = "chris";
	this->_serverName = SERVERNAME;
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
	BotCommand(str, this).execute();
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

bool	Bot::createGame(std::string const& target, std::string const gameName)
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

void	Bot::deleteGame(std::string const& gameName)
{
	std::map<std::string, Morfi*>::iterator	game;

	game = this->_morfiGames.find(gameName);
	if (game != this->_morfiGames.end())
	{
		delete game->second;
		this->_morfiGames.erase(game);
	}
}

void	Bot::checkGames(void)
{
	std::map<std::string, Morfi*>::iterator	buffer;
	std::map<std::string, Morfi*>::iterator	next;
	time_t									cTime;

	next = this->_morfiGames.begin();
	cTime = std::time(NULL);
	while (next != this->_morfiGames.end())
	{
		buffer = next;
		next++;
		if (cTime - buffer->second->getLastTime() > GAME_MAX_TIME)
			this->deleteGame(buffer->first);
	}
}

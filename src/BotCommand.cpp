/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:43:16 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 18:58:25 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <BotCommand.hpp>

BotCommand::BotCommand(std::string msg, Bot *cbot): _cbot(cbot)
{
	std::cout << "Bot cmd" << std::endl;
	ft_split_word(msg, this->_args);
	this->_user = this->getUserName(this->getArg());
	this->_cmd = this->getArg();
}

BotCommand::~BotCommand(void)
{
}

std::string	BotCommand::getArg(void)
{
	return (ft_getArg(this->_args));
}
void	BotCommand::sendPrivmsg(std::string const& target,
	std::string const& msg)
{
	this->_cbot->addCommandBuffer("PRIVMSG " + target + " :" + msg + "\n");
}

bool	BotCommand::checkCMD(std::string& cmd, std::string const& cmp)
{
	for (size_t i = 0; i < cmp.length() && i < cmd.length(); i++)
		cmd[i] = std::toupper(cmd[i]);
	return (cmd == cmp || !cmd.compare(0, cmp.length() + 1,
		cmp + " ", 0, cmp.length() + 1));
}

void	BotCommand::privmsg(void)
{
	std::string	dest;
	std::string	msg;

	dest = this->getArg();
	msg = this->getArg();
	if (msg[0] == ':')
		msg.erase(0, 1);
	if (dest[0] != '#')
		dest = this->_user;
	if (this->checkCMD(msg, "MORFI"))
		this->game(msg, dest);
	else if (this->_user == BOT_NAME && this->checkCMD(msg, "ROUND"))
		this->gameRound(msg);
	else if (this->_user != BOT_NAME && checkCMD(msg, "DAVID"))
		this->sendPrivmsg(dest, "oh NO aHHHHHHHHHHHHHH");
}

void	BotCommand::invite(void)
{
	std::string	channelTarget;

	this->getArg();
	channelTarget = this->getArg();
	this->_cbot->addCommandBuffer("JOIN " + channelTarget
		+ " " + this->_user + "\n");
	//this->sendPrivmsg(this->_user, "tkt frefro");
}

void	BotCommand::badJoinChan(void)
{
	std::string	channel;
	std::string	user;

	channel = this->getArg();
	user = this->getArg();
	this->sendPrivmsg(user, "you are not an operator of the channel "
		+ channel);
}

void	BotCommand::part(void)
{
	if (this->_user != BOT_NAME)
		this->_cbot->addCommandBuffer("PART " + this->getArg() + " :check\n");
}

void	BotCommand::morfiGamePlace(Morfi *game, std::string const& gameName)
{
	std::string		pos;
	int				height;
	int				width;
	unsigned int	i;

	height = -1;
	width = -1;
	pos = this->getArg();
	i = 0;
	if (pos.length() >= 2)
	{
		while (i < 2)
		{
			if (pos[i] >= '0' && pos[i] <= '2')
				width = pos[i] - '0';
			if (pos[i] >= 'A' && pos[i] <= 'C')
				height = pos[i] - 'A';
			i++;
		}
	}
	if (game->place(width, height) == SUCCESS)
		this->sendRound(gameName);
	else
		this->sendPrivmsg(this->_user, "BAD CASE");
}

void	BotCommand::morfiGameAct(std::string& arg, std::string& gameName)
{
	std::map<std::string, Morfi*>::iterator	game;

	game = this->_cbot->getMorfi().find(gameName);
	if (game != this->_cbot->getMorfi().end())
	{
		if (arg == "P" || arg == "PLACE")
		{
			if (this->_user == game->second->getUserRound())
				this->morfiGamePlace(game->second, gameName);
			else
				this->sendPrivmsg(this->_user, "is not your turn");
		}
		else
			this->sendPrivmsg(this->_user, "invalid argument");
	}
	else
		this->sendPrivmsg(this->_user, "invalide game name");
}

std::string	BotCommand::getLineMorfi(int const *grid, int line) const
{
	std::string		lineRet;
	unsigned int	i;

	i = 0;
	if (line == -1)
	{
		lineRet = "| 0 | 1 | 2 |  ";
	}
	else
	{
		lineRet = "| ~ | ~ | ~ | A";
		lineRet[14] += line;
		while (i < 3)
		{
			if (grid[i + line * 3])
				lineRet[2 + i * 4] = PLACE(grid[i + line * 3]);
			i++;
		}
	}
	return (lineRet);
}

void	BotCommand::sendRoundHeader(std::string const& gameName, Morfi *morfi,
	std::string const& targets)
{
	int			gameWin;
	std::string	buffer;

	gameWin = morfi->finished();
	if (gameWin == MO_P1 || gameWin == MO_P2 || gameWin == MO_NULL)
	{
		if (gameWin == MO_P1)
			buffer = morfi->getP1();
		else if (gameWin == MO_P2)
			buffer = morfi->getP2();
		if (buffer.empty())
			this->sendPrivmsg(targets, "FINISH NO WINNER FOR " + gameName);
		else
			this->sendPrivmsg(targets, "FINISH WINNER "
				+ buffer + " FOR " + gameName);
		this->_cbot->deleteGame(gameName);
	}
	else
		this->sendPrivmsg(targets, "ROUND " + morfi->getUserRound() + " FOR "
			+ gameName);
}

void	BotCommand::sendRound(std::string const& gameName)
{
	int const	*grid;
	std::string	targets;
	Morfi*		morfi = this->_cbot->getMorfi()[gameName];

	grid = morfi->getGrid();
	targets = morfi->getP1() + "," + morfi->getP2();
	this->sendRoundHeader(gameName, morfi, targets);
	this->sendPrivmsg(targets, this->getLineMorfi(grid, -1));
	this->sendPrivmsg(targets, this->getLineMorfi(grid, 0));
	this->sendPrivmsg(targets, this->getLineMorfi(grid, 1));
	this->sendPrivmsg(targets, this->getLineMorfi(grid, 2));
}

bool	BotCommand::morfiCheckcreateGame(void)
{
	bool	ret;

	ret = true;
	this->_cbot->checkGames();
	if (this->_cbot->getMorfi().size() > MAX_MORFI_GAMES)
	{
		this->sendPrivmsg(this->_user,
			"he's got too much game on, come back later");
		ret = false;
	}
	return (ret);
}

void	BotCommand::morfiGame(void)
{
	std::string	arg;
	std::string	gameName;

	if (this->_args.size() >= 1)
	{
		arg = this->getArg();
		gameName = this->getArg();
		if (arg == "NEW" && !gameName.empty() && this->morfiCheckcreateGame())
		{
			if (this->_cbot->createGame(this->_user, gameName))
			{
				this->sendPrivmsg(this->_user,
					"a new part has been created with "
					+ this->_cbot->getNickName());
				this->sendRound(gameName);
			}
			else
				this->sendPrivmsg(this->_user, "an error was occured");
		}
		else
			this->morfiGameAct(arg, gameName);
	}
	else
	{
		this->sendPrivmsg(this->_user, ERROR_CREAT_GAME);
		this->sendPrivmsg(this->_user, ERROR_PLACE_CASE);
	}
}

void	BotCommand::execute(void)
{
	if (this->_cmd == "PRIVMSG")
		this->privmsg();
	else if (this->_cmd == "ROUND")
		this->playRound();
	else if (this->_cmd == "MORFI")
		this->morfiGame();
	if (this->_user == BOT_NAME)
		return ;
	if (this->_cmd == "INVITE")
		this->invite();
	else if (this->_cmd == "BADJOINCHAN")
		this->badJoinChan();
	else if (this->_cmd == "PART")
		this->part();
}

void	BotCommand::playRound(void)
{
	std::string								gameName;
	std::map<std::string, Morfi*>::iterator	game;

	if (this->getArg() == BOT_NAME)
	{
		this->getArg();
		gameName = this->getArg();
		game = this->_cbot->getMorfi().find(gameName);
		if (game != this->_cbot->getMorfi().end())
			this->playRoundAct(game->second, gameName);
	}
}

void	BotCommand::playRoundActFormat(unsigned int i,
	std::string const& gameName)
{
	std::string	rplMorfi;
	char		act[3];

	rplMorfi = "MORFI P " + gameName + " ";
	act[0] = '0' + i % 3;
	act[1] = 'A' + i / 3;
	act[2] = '\0';
	rplMorfi += act;
	this->sendPrivmsg(this->_cbot->getNickName(), rplMorfi);
}

void	BotCommand::playRoundActPush(std::string const& gameName,
	int const *grid, int act)
{
	unsigned int	i;

	std::srand(std::time(NULL));
	act = std::rand() % act;
	i = 0;
	while (i < GRID_SIZE)
	{
		if (!grid[i] && act > 0)
			act--;
		else if (!grid[i])
		{
			this->playRoundActFormat(i, gameName);
			i = GRID_SIZE;
		}
		i++;
	}
}

void	BotCommand::playRoundAct(Morfi *game, std::string const& gameName)
{
	int const		*grid;
	unsigned int	act;
	unsigned int	i;

	grid = game->getGrid();
	i = 0;
	act = 0;
	while (i < GRID_SIZE)
	{
		if (!grid[i])
			act++;
		i++;
	}
	if (act)
		this->playRoundActPush(gameName, grid, act);
}

std::string	BotCommand::getUserName(std::string const& user) const
{
	std::string	userName;
	size_t		start;

	userName = "";
	if (user.empty() == false)
	{
		start = (user[0] == ':')?1 : 0;
		userName = user.substr(start, user.find_first_of('!') - start);
	}
	return (userName);
}

void	BotCommand::game(std::string const& params, std::string const& target)
{
	BotCommand(target + " " + params, this->_cbot).execute();
}

void	BotCommand::gameRound(std::string const& params)
{
	BotCommand(this->_cbot->getNickName()
		+ " " + params, this->_cbot).execute();
}

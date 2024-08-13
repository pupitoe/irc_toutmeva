/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:43:16 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/13 17:06:22 by tlassere         ###   ########.fr       */
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

bool	BotCommand::checkCMD(std::string cmd, std::string const& cmp)
{
	for (size_t i = 0; i < cmp.length() && i < cmd.length(); i++)
		cmd[i] = std::toupper(cmd[i]);
	return (cmd == cmp || !cmd.compare(0, cmp.length() + 1,
		cmp + " ", 0, cmp.length() + 1));
}

void	BotCommand::privmsg(void)
{
	std::string	dest;
	std::string	buffer;
	std::string	msg;

	dest = this->_user;
	buffer = this->getArg();
	msg = this->getArg();
	buffer = "hello zizou";
	if (msg[0] == ':')
		msg.erase(0, 1);
	if (buffer[0] == '#')
		dest = buffer;
	if (this->checkCMD(msg, "MORFI"))
		buffer = "how to basic";
	this->sendPrivmsg(dest, buffer);
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

void	BotCommand::execute(void)
{
	if (this->_cmd == "PRIVMSG")
		this->privmsg();
	else if (this->_cmd == "INVITE")
		this->invite();
	else if (this->_cmd == "BADJOINCHAN")
		this->badJoinChan();
	else if (this->_cmd == "PART")
		this->part();
	if (this->_user == BOT_NAME)
		std::cout << "oh non pas le bot :((((" << std::endl;
	else
		std::cout << "ah oui c'est un autre user !!!!" << std::endl;
}

std::string	BotCommand::getUserName(std::string const& user) const
{
	std::string	userName;
	size_t		start;

	userName = "";
	if (user.empty() == false)
	{
		start = (user[0] == ':')?1 : 0;
		userName = user.substr(start, userName.find_first_of('!') - start);
	}
	return (userName);
}
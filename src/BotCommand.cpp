/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:43:16 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 20:24:28 by tlassere         ###   ########.fr       */
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

void	BotCommand::privmsg(void)
{
	std::string	dest;
	std::string	buffer;

	dest = this->_user;
	buffer = this->getArg();
	if (buffer[0] == '#')
		dest = buffer;
	buffer = "hello zizou";
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

void	BotCommand::execute(void)
{
	if (this->_cmd == "PRIVMSG")
		this->privmsg();
	else if (this->_cmd == "INVITE")
		this->invite();
	else if (this->_cmd == "BADJOINCHAN")
		this->badJoinChan();
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

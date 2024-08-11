/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:43:16 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 01:14:54 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <BotCommand.hpp>

BotCommand::BotCommand(std::string msg) : Command(msg)
{
	//TODO Verifier ici les cmmd;
	this->_type = CHANNEL;
	std::cout << "channel creation" << std::endl;
	this->_user = this->getUserName(this->getArg());
	this->_cmd = this->getArg();
}

BotCommand::~BotCommand(void)
{
}

int BotCommand::execute(Client &)
{
	if (this->_user == BOT_NAME)
		std::cout << "oh non pas le bot :((((" << std::endl;
	else
		std::cout << "ah oui c'est un autre user !!!!" << std::endl;
	return (SUCCESS);
}

int	BotCommand::execute(Client *, std::map<std::string, Channel *>&,
	std::map<int, Client *>&)
{
	return (SUCCESS);
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

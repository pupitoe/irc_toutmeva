/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:43:16 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 15:11:21 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <BotCommand.hpp>

BotCommand::BotCommand(std::string msg)
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

int BotCommand::execute(void)
{
	if (this->_user == BOT_NAME)
		std::cout << "oh non pas le bot :((((" << std::endl;
	else
		std::cout << "ah oui c'est un autre user !!!!" << std::endl;
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

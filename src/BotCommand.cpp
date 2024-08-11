/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:43:16 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 00:51:15 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <BotCommand.hpp>

BotCommand::BotCommand(std::string msg) : Command(msg)
{
	//TODO Verifier ici les cmmd;
	this->_type = CHANNEL;
	std::cout << "channel creation" << std::endl;
}

BotCommand::~BotCommand(void)
{
}

int BotCommand::execute(Client &)
{
	
	return (SUCCESS);
}
int	BotCommand::execute(Client *, std::map<std::string, Channel *>&,
	std::map<int, Client *>&)
{
	return (SUCCESS);
}
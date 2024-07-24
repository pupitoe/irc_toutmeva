/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 17:35:26 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>
#include <iostream>

ChannelCommand::ChannelCommand(std::string msg) : Command(msg) {
	//TODO Verifier ici les cmmd;
	this->_type = CHANNEL;
	std::cout << "channel creation" << std::endl;
}

ChannelCommand::~ChannelCommand(void) {
	
}

int ChannelCommand::execute(int socket) {
	(void) socket;
	return (0);
}

int	ChannelCommand::execute(Client *client,
	std::map<std::string, Channel *>& channels)
{
	std::stringstream	ss(this->_msg);
	std::string			buffer;

	ss >> buffer;
	std::cout << "kk glac" << std::endl;
	if (buffer == "JOIN")
		std::cout << "hello word" << std::endl;
	(void)client;
	(void)channels;
	return (0);
}

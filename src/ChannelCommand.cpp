/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/25 19:25:34 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

ChannelCommand::ChannelCommand(std::string msg) : Command(msg) {
	//TODO Verifier ici les cmmd;
	this->_type = CHANNEL;
	std::cout << "channel creation" << std::endl;
}

int	ChannelCommand::execute(int)
{
	return (0);
}

ChannelCommand::~ChannelCommand(void) {
	
}

void	ChannelCommand::errorMessage(int error, Client *client)
{
	switch (error)
	{
	case ERR_NEEDMOREPARAMS:
		client->addRPLBuffer("<client> <command> :Not enough parameters\n");
		break;
	case ERR_NOSUCHCHANNEL:
		client->addRPLBuffer("<client> <channel> :No such channel\n");
	default:
		break;
	}
}

int	ChannelCommand::execute(Client *client,
	std::map<std::string, Channel *>& channels)
{
	std::stringstream	ss(this->_msg);
	std::string			buffer;

	ss >> buffer;
	if (buffer == "JOIN")
		return (this->join(client, channels, ss));
	if (buffer == "PART")
		return (this->part(client, channels, ss));
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelErrorMessages.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:25:25 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 20:24:51 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

void	Channel::RPL_CREATIONTIME(Client* client_rqst)
{
	client_rqst->addRPLBuffer(":irctoutmevas 329 "
		+ client_rqst->getNickName() + " "
		+ this->_name + " " + this->_creation_time + "\n");
}

void	Channel::ERR_CHANOPRIVSNEEDED_MSG(Client *client)
{
	client->addRPLBuffer(":irctoutmevas 482 " + client->getNickName()
		+ " " + this->_name + " :You're not channel operator\n");
}

void	Channel::ERR_NOTONCHANNEL_MSG(Client *client)
{
	client->addRPLBuffer(":irctoutmevas 442 " + client->getNickName()
		+ " " + this->_name + " :You're not channel operator\n");
}

void	Channel::ERR_USERNOTINCHANNEL_MSG(Client *client,
	std::string const& nick)
{
	client->addRPLBuffer(":irctoutmevas 441 " + client->getNickName() +
		" " + nick +  + " " + this->_name + " :They aren't on that channel\n");
}

void	Channel::ERR_USERONCHANNEL_MSG(Client *client,
	std::string const& nick)
{
	client->addRPLBuffer(":irctoutmevas 443 " + client->getNickName() +
		" " + nick +  + " " + this->_name + " :is already on channel\n");
}

void	Channel::ERR_BAT_JOIN_BOT(Client *client, std::string const& nick,
	std::string const& channel)
{
	client->addRPLBuffer(":irctoutmevas BADJOINCHAN " + channel + " "
		+ nick + "\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelErrorMessages.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:25:25 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/05 18:25:58 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

void	Channel::RPL_CREATIONTIME(Client* client_rqst)
{
	client_rqst->addRPLBuffer(":329 " + client_rqst->getNickName() + " "
		+ this->_name + " " + this->_creation_time + "\n");
}

void	Channel::ERR_CHANOPRIVSNEEDED_MSG(Client *client)
{
	client->addRPLBuffer(":482 " + client->getNickName()
		+ " " + this->_name + " :You're not channel operator\n");
}

void	Channel::ERR_NOTONCHANNEL_MSG(Client *client)
{
	client->addRPLBuffer(":442 " + client->getNickName()
		+ " " + this->_name + " :You're not channel operator\n");
}

void	Channel::ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick)
{
	client->addRPLBuffer(":401 " + client->getNickName() +
		" " + nick + " :No such nick\n");
}

void	Channel::ERR_USERNOTINCHANNEL_MSG(Client *client,
	std::string const& nick)
{
	client->addRPLBuffer(":441 " + client->getNickName() +
		" " + nick +  + " " + this->_name + " :They aren't on that channel\n");
}
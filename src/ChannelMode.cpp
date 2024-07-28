/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:51:08 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/29 01:37:52 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

int	Channel::mode_t(Client* client_rqst, int signe)
{
	int status;
	status = ERR_CHANOPRIVSNEEDED;
	if (this->inOpLst(client_rqst))
	{
		status = 0;
		if (this->_topic_priv_need == signe)
			this->mode(client_rqst);
		else
			this->sendAll(":" + client_rqst->getNickName() + " MODE " +
				this->_name + " " + ((signe)? "+": "-") + "t\n");
		this->_topic_priv_need = signe;
	}
	else
		client_rqst->addRPLBuffer("482\n");
	return (status);
}

int	Channel::mode(Client* client_rqst)
{
	std::string	buffer;

	buffer = ":324 " + client_rqst->getNickName() + " " + this->_name;
	buffer += "\n";
	client_rqst->addRPLBuffer(buffer);
	RPL_CREATIONTIME(client_rqst);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:51:08 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/03 16:10:28 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

#include <iostream>

int	Channel::modeBasic(bool *modeVar, int signe, char typeMode,
	Client *client_rqst)
{
	int status;

	status = ERR_CHANOPRIVSNEEDED;
	if (this->inOpLst(client_rqst))
	{
		status = 0;
		if (*modeVar == signe)
			this->mode(client_rqst);
		else
			this->sendAll(":" + client_rqst->getNickName() + " MODE " +
				this->_name + " " + ((signe)? "+": "-") + typeMode + "\n");
		*modeVar = signe;
	}
	else
		client_rqst->addRPLBuffer("482\n");
	return (status);
}

int	Channel::mode_i(Client* client_rqst, int signe)
{
	return (this->modeBasic(&this->_invite_only, signe, 'i', client_rqst));
}

int	Channel::mode_t(Client* client_rqst, int signe)
{
	return (this->modeBasic(&this->_topic_priv_need, signe, 't', client_rqst));
}

// cas non gerer: s'il n'y a pas dargument pour l'username il vas renvoyer une erreur (dans se cas il ne dois rien print)
int	Channel::mode_o(Client* client_rqst, int signe, std::string& user)
{
	int		status;
	Client *target_client;

	status = ERR_CHANOPRIVSNEEDED;
	if (this->inOpLst(client_rqst))
	{
		status = 0;
		target_client = this->getClient(user);
		if (target_client)
		{
			if (signe && inOpLst(target_client) == false)
				this->_operators.push_front(target_client);
			else if (signe == 0 && inOpLst(target_client) == true)
				this->_operators.erase(std::find(this->_operators.begin(),
				this->_operators.end(), target_client));
			else
				return (status);
			this->sendAll(":" + client_rqst->getNickName() + " MODE " +
				this->_name + " " + ((signe)? "+": "-") + "o " + user + "\n");
		}
		else
			client_rqst->addRPLBuffer("441\n");
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

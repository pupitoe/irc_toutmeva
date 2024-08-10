/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:51:08 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/10 19:19:16 by tlassere         ###   ########.fr       */
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
		if (*modeVar != signe)
			this->sendAll(":" + client_rqst->getNickName() + " MODE " +
				this->_name + " " + ((signe)? "+": "-") + typeMode + "\n");
		*modeVar = signe;
	}
	else
		this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
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

int	Channel::mode_o(Client* client_rqst, int signe, std::string const& user)
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
			else if (signe == LESS && inOpLst(target_client) == true)
				this->_operators.erase(std::find(this->_operators.begin(),
				this->_operators.end(), target_client));
			else
				return (status);
			this->sendAll(":" + client_rqst->getNickName() + " MODE " +
				this->_name + " " + ((signe)? "+": "-") + "o " + user + "\n");
		}
		else
			this->ERR_USERNOTINCHANNEL_MSG(client_rqst, user);
	}
	else
		this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
	return (status);
}

void	Channel::RPL_MODE_L(Client *client_rqst)
{
	std::stringstream	ss;
	std::string 		buffer;

	ss << this->_limit;
	ss >> buffer;
	this->sendAll(":" + client_rqst->getNickName() + " MODE " +
		this->_name + " +l " + buffer + "\n");
}

int	Channel::mode_l(Client* client_rqst, int signe, std::string const& limit)
{
	int			status;
	long		buffer;

	status = ERR_CHANOPRIVSNEEDED;
	if (this->inOpLst(client_rqst))
	{
		status = SUCCESS;
		this->_limit = 0;
		if (signe)
		{
			buffer = std::strtol(limit.c_str(), NULL, 10);
			if (buffer > LIMIT_USER_IN_CHANNEL
				|| limit.find_first_not_of("0123456789") < limit.length())
				buffer = 0;
			this->_limit = buffer;
			this->RPL_MODE_L(client_rqst);
		}
		else
			this->sendAll(":" + client_rqst->getNickName() +
				" MODE " + this->_name + " -l\n");
	}
	else
		this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
	return (status);
}

int	Channel::mode_k_analyse(Client* client_rqst, std::string const& key)
{
	int			status;
	std::string	error;
	std::string	parameter;

	status = SUCCESS;
	parameter = "*";
	if (key.empty())
		error = "the key must not be empty\n";
	if (error.empty() && key.find_first_of(": ,\t\v") < key.length())
		error = "the key must not have space, comma and colon\n";
	if (error.empty() && key.length() > 32)
	{
		error = "the key must have no more than 32 characters\n";
		parameter = key;
	}
	if (error.empty() == false)
	{
		status = FAIL;
		client_rqst->addRPLBuffer(": 696 " + client_rqst->getNickName() + " "
			+ this->_name + " k "+ parameter + " :" + error);
		client_rqst->addRPLBuffer(": 525 " + client_rqst->getNickName() + " "
			+ this->_name + " :Key is not well-formed\n");
	}
	return (status);
}

int	Channel::mode_k(Client* client_rqst, int signe, std::string const& key)
{
	int			status;

	status = ERR_CHANOPRIVSNEEDED;
	if (this->inOpLst(client_rqst))
	{
		status = 0;
		if (signe)
		{
			status = mode_k_analyse(client_rqst, key);
			if (status == SUCCESS)
			{
				this->_key = key;
				this->sendAll(":" + client_rqst->getNickName()
					+ " MODE " + this->_name + " +k " + key + "\n");
			}
		}
		else
		{
			this->_key.erase();
			this->sendAll(":" + client_rqst->getNickName()
				+ " MODE " + this->_name + " -k *\n");
		}
	}
	else
		this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
	return (status);
}

int	Channel::mode(Client* client_rqst)
{
	std::stringstream	ss;
	std::string			args;
	std::string			modes;

	modes = "+";
	args = "";
	if (this->_key.empty() == false && this->inLst(client_rqst))
	{
		modes += 'k';
		args +=  " " + this->_key;
	}
	if (this->_limit)
	{
		ss << this->_limit;
		modes += 'l';
		args += " " + ss.str();
	}
	if (this->_topic_priv_need)
		modes += 't';
	if (this->_invite_only)
		modes += 'i';
	client_rqst->addRPLBuffer(": 324 " + client_rqst->getNickName() + " "
		+ this->_name + " :" + ((modes.length() > 1)? modes: "") + args + "\n");
	RPL_CREATIONTIME(client_rqst);
	return (SUCCESS);
}

bool Channel::mode_t_signe(void) const
{
	return (this->_topic_priv_need);
}

bool Channel::mode_i_signe(void) const
{
	return (this->_invite_only);
}

int Channel::mode_other(Client *client_rqst, char flag)
{
	int			status;

	status = ERR_CHANOPRIVSNEEDED;
	if (this->inOpLst(client_rqst))
	{
		status = SUCCESS;
		client_rqst->addRPLBuffer(": 472 " + client_rqst->getNickName() + " "
			+ flag + " :is unknown mode char to me\n");
	}
	else
		this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
	return (status);
}

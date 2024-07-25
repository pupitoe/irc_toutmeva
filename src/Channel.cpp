/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:47:12 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/26 00:03:48 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel(std::string const& name): _name(name)
{
	this->_super_user_set = false;
}

Channel::~Channel(void)
{
}

int	Channel::part(Client *client_rqst)
{
	std::list<Client *>::iterator	buffer;
	std::list<Client *>::iterator	buffer_op;

	buffer = std::find(this->_client.begin(), this->_client.end(), client_rqst);
	buffer_op = std::find(this->_operators.begin(),
		this->_operators.end(), client_rqst);
	if (buffer == this->_client.end())
	{
		client_rqst->addRPLBuffer(":442 " + client_rqst->getNickName() +
			+ " " + this->_name + " :You're not on that channel\n");
		return (ECHAN_NOT_REGISTERED);
	}
	this->_client.erase(buffer);
	if (buffer_op != this->_operators.end())
		this->_operators.erase(buffer_op);
	return (GOOD_PART);
}

int	Channel::inLst(Client *client)
{
	return (std::find(this->_client.begin(), this->_client.end(), client)
		!= this->_client.end());
}

int	Channel::inOpLst(Client *client)
{
	return (std::find(this->_operators.begin(), this->_operators.end(), client)
		!= this->_operators.end());
}

void	Channel::RPL_NAMREPLY(Client *client)
{
	std::string						buffer;
	std::list<Client *>::iterator	it;
	buffer = ":353 " + client->getNickName() + " = " + this->_name + " :";
	it = this->_client.begin();
	while (it != this->_client.end())
	{
		if (it != this->_client.end())
			buffer += " ";
		if (this->inOpLst(*it))
			buffer += "@";
		buffer += client->getNickName();
			
		it++;
	}
	buffer += "\n";
	client->addRPLBuffer(buffer);
}

void	Channel::RPL_ENDOFNAMES(Client *client)
{
	std::string						buffer;
	buffer = ":366 " + client->getNickName() + " " + this->_name
		+ " :End of NAMES list\n";
	client->addRPLBuffer(buffer);
}

int	Channel::join(Client *client_rqst)
{
	if (this->inLst(client_rqst))
		return (ECHAN_ALREADY_REGISTERED);
	if (this->_super_user_set == false)
	{
		this->_operators.push_back(client_rqst);
		this->_super_user_set = true;
	}
	this->_client.push_back(client_rqst);
	this->RPL_NAMREPLY(client_rqst);
	this->RPL_ENDOFNAMES(client_rqst);
	return (GOOD_REGISTER);
}

int	Channel::userGrade(std::string const& nickName)
{
	int								grade;
	std::list<Client *>::iterator	it;
	std::list<Client *>::iterator	itend;

	grade = CH_NO_GRADE;
	it = this->_client.begin();
	itend = this->_client.end();
	while (it != itend && (*it)->getNickName() != nickName)
		it++;
	if (it != itend)
		grade = CH_USER + this->inOpLst(*it);
	return (grade);
}

int	Channel::kick(Client* client_rqst, std::string const& userKick,
	std::string const& comment)
{
	int status;

	status = 1;
	if (this->inLst(client_rqst))
	{
		if (this->userGrade(userKick))
		{
			if (this->inOpLst(client_rqst) == CH_OPERATOR)
			{
				status = 0;
				// make kick messages !!!!!!!
			}
			else
				client_rqst->addRPLBuffer("482");
		}
		else
			client_rqst->addRPLBuffer("441");

	}
	else
		client_rqst->addRPLBuffer("442");
	(void)comment;
	return (status);
}

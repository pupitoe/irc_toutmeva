/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:47:12 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/26 19:34:50 by tlassere         ###   ########.fr       */
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

// add reson
int	Channel::part(Client *client_rqst, std::string const& reason)
{
	std::list<Client *>::iterator	buffer;

	buffer = std::find(this->_client.begin(), this->_client.end(), client_rqst);
	if (buffer == this->_client.end())
	{
		client_rqst->addRPLBuffer(":442 " + client_rqst->getNickName() +
			+ " " + this->_name + " :You're not on that channel\n");
		return (ECHAN_NOT_REGISTERED);
	}
	this->sendAll(":" + client_rqst->getNickName() + " PART " + this->_name +
		((reason.empty())? "": " " + reason) + "\n");
	this->_client.erase(buffer);
	buffer = std::find(this->_operators.begin(),
		this->_operators.end(), client_rqst);
	if (buffer != this->_operators.end())
		this->_operators.erase(buffer);
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
	std::string	buffer;

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
	this->sendAll(":" + client_rqst->getNickName() + " JOIN " + this->_name
		+ "\n");
	this->RPL_NAMREPLY(client_rqst);
	this->RPL_ENDOFNAMES(client_rqst);
	return (GOOD_REGISTER);
}

int	Channel::userGrade(std::string const& nickName)
{
	int		grade;
	Client	*buffer;

	grade = CH_NO_GRADE;
	buffer = this->getClient(nickName);
	if (buffer)
		grade = CH_USER + this->inOpLst(buffer);
	return (grade);
}

Client	*Channel::getClient(std::string const& nickName)
{
	std::list<Client *>::iterator	it;
	std::list<Client *>::iterator	itend;
	Client							*buffer;

	it = this->_client.begin();
	itend = this->_client.end();
	while (it != itend && (*it)->getNickName() != nickName)
		it++;
	if (it == itend)
		buffer = NULL;
	else
		buffer = *it;
	return (buffer);
}

void	Channel::sendAll(std::string const& msg)
{
	std::list<Client *>::iterator	it;
	std::list<Client *>::iterator	itend;

	it = this->_client.begin();
	itend = this->_client.end();
	while (it != itend)
	{
		(*it)->addRPLBuffer(msg);
		it++;
	}
}

void	Channel::kickActiv(Client* client_rqst, std::string const& userKick,
	std::string const& comment)
{
	std::string	buffer;
	Client		*user;

	user = this->getClient(userKick);	
	if (user)
	{
		buffer = ":" + client_rqst->getNickName() + " KICK " + this->_name + " "
			+ user->getNickName() + " ";
		if (comment.empty())
			buffer += "a moderator kick u sorry\n";
		else
			buffer += comment + "\n";
		this->sendAll(buffer);
		this->_client.erase(std::find(this->_client.begin(),
			this->_client.end(), user));
		if (this->inOpLst(user))
			this->_operators.erase(std::find(this->_operators.begin(),
				this->_operators.end(), user));
	}
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
				this->kickActiv(client_rqst, userKick, comment);
			}
			else
				client_rqst->addRPLBuffer("482");
		}
		else
			client_rqst->addRPLBuffer("441");
	}
	else
		client_rqst->addRPLBuffer("442");
	return (status);
}

size_t	Channel::countClient(void) const
{
	return (this->_client.size());
}

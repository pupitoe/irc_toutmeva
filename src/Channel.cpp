/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:47:12 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/28 20:06:21 by tlassere         ###   ########.fr       */
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
		buffer += (*it)->getNickName();
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
	std::stringstream	ss;

	if (this->inLst(client_rqst))
		return (ECHAN_ALREADY_REGISTERED);
	if (this->_super_user_set == false)
	{
		ss << std::time(NULL);
		ss >> this->_creation_time;
		this->_operators.push_back(client_rqst);
		this->_super_user_set = true;
	}
	this->_client.push_back(client_rqst);
	this->sendAll(":" + client_rqst->getNickName() + " JOIN " + this->_name
		+ "\n");
	if (this->_topic.empty() == 0)
		this->topicRPL(client_rqst);
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
		buffer = ":" + client_rqst->getNickName() + " KICK "
			+ this->_name + " " + user->getNickName() + " ";
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
		if (this->getClient(userKick))
		{
			if (this->inOpLst(client_rqst))
			{
				status = 0;
				this->kickActiv(client_rqst, userKick, comment);
			}
			else
				client_rqst->addRPLBuffer("482\n");
		}
		else
			client_rqst->addRPLBuffer("441\n");
	}
	else
		client_rqst->addRPLBuffer("442\n");
	return (status);
}

void	Channel::topicRPL(Client *client_rqst)
{
	client_rqst->addRPLBuffer(":332 " + client_rqst->getNickName()
		+ " " + this->_name + " :" + this->_topic + "\n");
	client_rqst->addRPLBuffer(":333 " + client_rqst->getNickName()
		+ " " + this->_name + " " + this->_topic_usr + "\n");
}

void	Channel::topicChange(Client* client_rqst, std::string const& newTopic)
{
	std::stringstream	ss;
	std::string			buffer;

	if (newTopic.empty())
	{
		this->_topic_usr.erase();
		this->_topic.erase();
	}
	else
	{
		ss << std::time(NULL); 
		ss >> buffer;
		this->_topic = newTopic.c_str() + ((newTopic[0] == ':')? 1: 0);
		this->_topic_usr = client_rqst->getNickName() + " " + buffer;
		this->sendAll(":" + client_rqst->getNickName()
			+ " TOPIC " + this->_name + " :" + this->_topic + "\n");
	}
}

void	Channel::topicActiv(Client* client_rqst, std::string const& newTopic,
	int topicHaveArg)
{
	if (topicHaveArg == 0)
	{
		if (this->_topic.empty())
			client_rqst->addRPLBuffer(":331 " + client_rqst->getNickName()
				+ " " + this->_name + " :No topic is set\n");
		else
			this->topicRPL(client_rqst);
	}
	else
		this->topicChange(client_rqst, newTopic);
}

int	Channel::topic(Client* client_rqst, std::string const& newTopic,
	int topicHaveArg)
{
	int status;

	status = 1;
	if (this->inLst(client_rqst))
	{
		if (newTopic.empty() || this->inOpLst(client_rqst))
		{
			status = 0;
			this->topicActiv(client_rqst, newTopic, topicHaveArg);
		}
		else
			client_rqst->addRPLBuffer("482\n");
	}
	else
		client_rqst->addRPLBuffer("442\n");
	return (status);
}

size_t	Channel::countClient(void) const
{
	return (this->_client.size());
}

bool	channelExist(std::string const& channelName,
	std::map<std::string, Channel *>& channels)
{
	return (channels.find(channelName) != channels.end());
}

void	closeChannel(std::string const& channelName,
	std::map<std::string, Channel *>& channels)
{
	if (channelExist(channelName, channels)
		&& channels[channelName]->countClient() == 0)
	{
		delete channels[channelName];
		channels.erase(channelName);
	}
}

void	Channel::RPL_CREATIONTIME(Client* client_rqst)
{
	client_rqst->addRPLBuffer(":329 " + client_rqst->getNickName() + " "
		+ this->_name + " " + this->_creation_time + "\n");
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

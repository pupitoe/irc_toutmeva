/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:47:12 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/05 18:16:28 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel(std::string const& name): _name(name)
{
	this->_super_user_set = false;
	this->_topic_priv_need = true;
	this->_invite_only = false;
	this->_limit = 0;
}

Channel::~Channel(void)
{
}

int	Channel::part(Client *client_rqst, std::string const& reason,
	bool quitServe)
{
	std::list<Client *>::iterator	buffer;

	buffer = std::find(this->_client.begin(), this->_client.end(), client_rqst);
	if (buffer == this->_client.end())
	{
		if (quitServe == false)
			this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
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

int	Channel::inLst(Client *client) const
{
	return (std::find(this->_client.begin(), this->_client.end(), client)
		!= this->_client.end());
}

int	Channel::inOpLst(Client *client) const
{
	return (std::find(this->_operators.begin(), this->_operators.end(), client)
		!= this->_operators.end());
}

int	Channel::inInvitLst(Client *client) const
{
	return (std::find(this->_invite_lst.begin(),
		this->_invite_lst.end(), client) != this->_invite_lst.end());
}

void	Channel::eraseInviteLst(Client *client)
{
	std::list<Client *>::iterator	it;

	it = std::find(this->_invite_lst.begin(), this->_invite_lst.end(), client);
	if (it != this->_invite_lst.end())
		this->_invite_lst.erase(it);
}

void	Channel::RPL_NAMREPLY(Client *client)
{
	std::string						buffer;
	std::list<Client *>::iterator	it;

	buffer = ":353 " + client->getNickName() + " = " + this->_name + " :";
	it = this->_client.begin();
	while (it != this->_client.end())
	{
		if (it != this->_client.begin())
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

void	Channel::join_super_user(Client* client_rqst)
{
	std::stringstream	ss;

	ss << std::time(NULL);
	ss >> this->_creation_time;
	this->_operators.push_back(client_rqst);
	this->_super_user_set = true;
}

void	Channel::RPL_JOIN_MSG_ERR(Client *client_rqst, std::string const& error,
	char type)
{
	client_rqst->addRPLBuffer(":" + error + " " + client_rqst->getNickName()
		+ " " + this->_name + " :Cannot join channel (+" + type +")\n");
}

int	Channel::join_check(Client *client_rqst, std::string const& key)
{
	if (this->inLst(client_rqst))
		return (ECHAN_ALREADY_REGISTERED);
	if (this->_limit && this->countClient() >= this->_limit)
	{
		this->RPL_JOIN_MSG_ERR(client_rqst, "471", 'l');
		return (ERR_CHANNELISFULL);
	}
	if (this->_invite_only && this->inInvitLst(client_rqst) == false)
	{
		this->RPL_JOIN_MSG_ERR(client_rqst, "473", 'i');
		return (ERR_INVITEONLYCHAN);
	}
	if (this->_invite_only == false && this->_key.empty() == false
		&& this->_key != key)
	{
		this->RPL_JOIN_MSG_ERR(client_rqst, "473", 'k');
		return (ERR_BADCHANNELKEY);
	}
	return (SUCCESS);
}

int	Channel::join(Client *client_rqst, std::string const& key)
{
	int status;

	status = this->join_check(client_rqst, key);
	if (status)
		return (status);
	if (this->_super_user_set == false)
		this->join_super_user(client_rqst);
	this->_client.push_back(client_rqst);
	this->sendAll(":" + client_rqst->getNickName() + " JOIN " + this->_name
		+ "\n");
	if (this->_topic.empty() == false)
		this->topicRPL(client_rqst);
	this->RPL_NAMREPLY(client_rqst);
	this->RPL_ENDOFNAMES(client_rqst);
	this->eraseInviteLst(client_rqst);
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

	buffer = NULL;
	it = this->_client.begin();
	itend = this->_client.end();
	while (it != itend && (*it)->getNickName() != nickName)
		it++;
	if (it != itend)
		buffer = *it;
	return (buffer);
}

Client	*getClientMap(std::string const& nickName,
	std::map<int, Client*>clientsLst)
{
	std::map<int, Client *>::iterator	it;
	std::map<int, Client *>::iterator	itend;
	Client								*buffer;

	buffer = NULL;
	it = clientsLst.begin();
	itend = clientsLst.end();
	while (it != itend && it->second->getNickName() != nickName)
		it++;
	if (it != itend)
		buffer = it->second;
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

	status = FAIL;
	if (this->inLst(client_rqst))
	{
		if (this->getClient(userKick))
		{
			if (this->inOpLst(client_rqst))
			{
				status = SUCCESS;
				this->kickActiv(client_rqst, userKick, comment);
			}
			else
				this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
		}
		else
			client_rqst->addRPLBuffer("441\n");
	}
	else
		this->ERR_NOTONCHANNEL_MSG(client_rqst);
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
		this->_topic = newTopic.c_str();
		this->_topic_usr = client_rqst->getNickName() + " " + buffer;
	}
	this->sendAll(":" + client_rqst->getNickName()
		+ " TOPIC " + this->_name + " :" + this->_topic + "\n");
}

void	Channel::topicActiv(Client* client_rqst, std::string const& newTopic,
	int topicHaveArg)
{
	if (topicHaveArg == false)
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

	status = FAIL;
	if (this->inLst(client_rqst))
	{
		if ((newTopic.empty() && topicHaveArg == 0)
			|| this->_topic_priv_need == 0 || this->inOpLst(client_rqst))
		{
			status = SUCCESS;
			this->topicActiv(client_rqst, newTopic, topicHaveArg);
		}
		else
			this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
	}
	else
		this->ERR_NOTONCHANNEL_MSG(client_rqst);
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

void	Channel::sendInvitClient(Client* client_rqst,
	std::string const& userName, std::map<int, Client *>& clientsLst)
{
	Client	*buffer;

	buffer = getClientMap(userName, clientsLst);
	if (buffer)
	{
		client_rqst->addRPLBuffer(":341 " + client_rqst->getNickName() +
			" " + userName + " " + this->_name + "\n");
		if (std::find(this->_invite_lst.begin(), this->_invite_lst.end(),
				buffer) == this->_invite_lst.end())
			this->_invite_lst.push_back(buffer);
		buffer->addRPLBuffer(":" + client_rqst->getNickName() + " INVITE "
			+ userName + " " + this->_name + "\n");
	}
	else
		this->ERR_NOSUCHNICK_MSG(client_rqst, userName);
}

int	Channel::invite(Client* client_rqst, std::string const& userName,
	std::map<int, Client *>& clientsLst)
{
	int status;

	status = FAIL;
	if (this->inLst(client_rqst))
	{
		if (this->inOpLst(client_rqst))
		{
			if (!this->getClient(userName))
			{
				status = SUCCESS;
				this->sendInvitClient(client_rqst, userName, clientsLst);
			}
			else
				client_rqst->addRPLBuffer("443\n");
		}
		else
			this->ERR_CHANOPRIVSNEEDED_MSG(client_rqst);
	}
	else
		this->ERR_NOTONCHANNEL_MSG(client_rqst);
	return (status);
}

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

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick)
{
	client->addRPLBuffer(":401 " + client->getNickName() +
		" " + nick + " :No such nick\n");
}

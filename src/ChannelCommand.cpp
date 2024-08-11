/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/12 00:59:37 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

ChannelCommand::ChannelCommand(std::string msg) : Command(msg) {
	//TODO Verifier ici les cmmd;
	this->_type = CHANNEL;
	std::cout << "channel creation" << std::endl;
}

int	ChannelCommand::execute(Client &)
{
	return (0);
}

ChannelCommand::~ChannelCommand(void) {
	
}

int	ChannelCommand::kick_channel(Client* user_rqts,
	std::string const& channelName, std::string const& userKick,
	std::string const& comment, std::map<std::string, Channel *>& channels)
{
	int		status;

	status = ERR_NOSUCHCHANNEL;
	if (channelExist(channelName, channels) == true)
	{
		status = channels[channelName]->kick(user_rqts, userKick, comment);
		closeChannel(channelName, channels);
	}
	return (status);
}

int	ChannelCommand::kick(Client *client,
	std::map<std::string, Channel *>& channels)
{
	int			status;
	size_t		i;
	std::string	channel;
	std::string	user_name;
	std::string	comment;
	std::string	buffer_user_name;

	channel = this->getArg();
	user_name = this->getArg();
	comment = this->getArg();
	i = 0;
	buffer_user_name = getPart(user_name, i);
	while (i < 100 && buffer_user_name.empty() == 0)
	{
		status = this->channelFormating(channel);
		if (status == SUCCESS)
			status = this->kick_channel(client, channel, buffer_user_name,
				comment, channels);
		this->errorMessage(status, client, channel);
		i++;
		buffer_user_name = getPart(user_name, i);
	}
	if (user_name.empty())
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, channel);
	return (SUCCESS);
}

int	ChannelCommand::topic_channel(Client* user_rqts,
	std::string const& channelName, std::string const& newTopic,
	int topicHaveArg, std::map<std::string, Channel *>& channels)
{
	int		status;

	status = ERR_NOSUCHCHANNEL;
	if (channelExist(channelName, channels) == true)
		status = channels[channelName]->topic(user_rqts,
			newTopic, topicHaveArg);
	return (status);
}

int	ChannelCommand::topic(Client *client,
	std::map<std::string, Channel *>& channels)
{
	int			status;
	int			topicHaveArg;
	std::string	channelName;
	std::string	newTopic;

	topicHaveArg = false;
	if (this->_args.size() > 1)
		topicHaveArg = true;
	channelName = this->getArg();
	newTopic = this->getArg();
	if (channelName.empty() == false)
	{
		status = this->channelFormating(channelName);
		if (status == SUCCESS)
			status = this->topic_channel(client,
				channelName, newTopic, topicHaveArg, channels);
		this->errorMessage(status, client, channelName);
	}
	else
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, channelName);
	return (0);
}

int	ChannelCommand::invite(Client *client, std::map<std::string,
	Channel *>& channels, std::map<int, Client *>& clientLst)
{
	int			status;
	std::string	userName;
	std::string	channelName;

	userName = this->getArg();
	channelName = this->getArg();
	if (channelName.empty() == 0)
	{
		status = this->channelFormating(channelName);
		if (status == SUCCESS)
		{
			status = ERR_NOSUCHCHANNEL;
			if (channelExist(channelName, channels))
				status = channels[channelName]->invite(client,
					userName, clientLst);
		}
		this->errorMessage(status, client, channelName);
	}
	else
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, channelName);
	return (SUCCESS);
}

int	ChannelCommand::privmsg_exec_channel(Client *client,
	std::map<std::string, Channel *>& channels,
	std::string const& target, std::string const& message)
{
	int			status;
	int			op;
	std::string	channelName;

	op = (target[0] == '@')? 1: 0;
	channelName = target.substr(op, target.length());
	status = this->channelFormating(channelName);
	if (status == SUCCESS)
	{
		status = ERR_NOSUCHCHANNEL;
		if (channelExist(channelName, channels))
			status = channels[channelName]->sendMsg(client, message, op);
	}
	return (status);
}

int	ChannelCommand::privmsg_exec_client(Client *client,
	std::map<int, Client *>& clientLst, std::string const& target,
	std::string const& message)
{
	int	status;
	Client	*buffer;

	status = std::atoi(ERR_NOSUCHNICK);
	buffer = getClientMap(target, clientLst);
	if (buffer)
	{
		status = SUCCESS;
		RPL_PRIVMSG(client, buffer, message, buffer->getNickName());
	}
	else
		ERR_NOSUCHNICK_MSG(client, target);
	return (status);
}

int	ChannelCommand::privmsg_exec(Client *client,
	std::map<std::string, Channel *>& channels,
	std::map<int, Client *>& clientLst, std::string const& target,
	std::string const& message)
{
	int	status;

	status = SUCCESS;
	if (!target.compare(0, 1, "#") || !target.compare(0, 2, "@#"))
		status = this->privmsg_exec_channel(client, channels, target, message);	
	else
		status = this->privmsg_exec_client(client, clientLst, target, message);
	return (status);
}

int	ChannelCommand::privmsg(Client *client, std::map<std::string,
	Channel *>& channels, std::map<int, Client *>& clientLst)
{
	int			status;
	size_t		i;
	std::string	targets;
	std::string	message;
	std::string	target_buffer;

	targets = this->getArg();
	message = this->getArg();
	i = 0;
	target_buffer = getPart(targets, i);
	while (i < 100 && target_buffer.empty() == false && !message.empty())
	{
		status = this->privmsg_exec(client, channels, clientLst,
			target_buffer, message);
		this->errorMessage(status, client, target_buffer);
		i++;
		target_buffer = getPart(targets, i);
	}
	if (this->_nb_arg >= 3 && message.empty())
		client->addRPLBuffer(":irctoutmevas 412 " + client->getNickName()
			+ " :No text to send");
	else if (message.empty())
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, targets);
	return (SUCCESS);
}

int	ChannelCommand::ping(Client *client)
{
	std::string	msg;

	if (this->_args.size() > 0)
	{
		msg = this->getArg();
		if (msg.empty())
			this->ERR_NOORIGIN_MSG(client);
		else
			client->addRPLBuffer("PONG irctoutmevas :" + msg + "\n");
	}
	else
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, "");
	return (SUCCESS);
}

int	ChannelCommand::pong(Client *client)
{
	std::string	msg;

	if (this->_args.size() > 0)
	{
		msg = this->getArg();
		if (msg.empty())
			this->ERR_NOORIGIN_MSG(client);
		else if (client->getSendPing() && this->getArg() == "coucou")
		{
			client->setLastPing(std::time(NULL));
			client->setSendPing(false);
		}
	}
	else
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, "");
	return (SUCCESS);
}

void	 ChannelCommand::ERR_NOORIGIN_MSG(Client *client)
{
	client->addRPLBuffer(":irctoutmevas 409 " + 
		client->getNickName() + " :No origin specified\n");
}

int	ChannelCommand::execute(Client *client, std::map<std::string,
	Channel *>& channels, std::map<int, Client *>& clientLst)
{
	std::string			buffer;

	//if (client->getStatusClient() != CS_CONNECTED)
	//	return (FAIL);
	buffer = this->getArg();
	if (buffer == "JOIN")
		return (this->join(client, channels));
	if (buffer == "PART")
		return (this->part(client, channels));
	if (buffer == "KICK")
		return (this->kick(client, channels));
	if (buffer == "TOPIC")
		return (this->topic(client, channels));
	if (buffer == "MODE")
		return (this->mode(client, channels));
	if (buffer == "INVITE")
		return (this->invite(client, channels, clientLst));
	if (buffer == "PRIVMSG")
		return (this->privmsg(client, channels, clientLst));
	if (buffer == "PING")
		return (this->ping(client));
	if (buffer == "PONG")
		return (this->pong(client));
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/28 20:16:44 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

ChannelCommand::ChannelCommand(std::string msg) : Command(msg) {
	//TODO Verifier ici les cmmd;
	this->_type = CHANNEL;
	std::cout << "channel creation" << std::endl;
}

int	ChannelCommand::execute(int)
{
	return (0);
}

ChannelCommand::~ChannelCommand(void) {
	
}

std::string	ChannelCommand::getArg(void)
{
	std::string	buffer;
	
	if (this->_args.size())
	{
		buffer = this->_args.front();
		this->_args.pop_front();
	}
	return (buffer);
}

void	ChannelCommand::errorMessage(int error, Client *client)
{
	switch (error)
	{
	case ERR_NEEDMOREPARAMS:
		client->addRPLBuffer("<client> <command> :Not enough parameters\n");
		break;
	case ERR_NOSUCHCHANNEL:
		client->addRPLBuffer("<client> <channel> :No such channel\n");
	default:
		break;
	}
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
		std::cout << getPart(user_name, i) << " with key: '"
			<< getPart(comment, i) << "'"<< std::endl;
		status = this->channelFormating(channel);
		if (status == SUCCESS)
			status = this->kick_channel(client, channel, buffer_user_name,
				comment, channels);
		this->errorMessage(status, client);
		i++;
		buffer_user_name = getPart(user_name, i);
	}
	if (user_name.empty())
		this->errorMessage(ERR_NEEDMOREPARAMS, client);
	return (0);
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

	topicHaveArg = 0;
	if (this->_args.size() > 1)
		topicHaveArg = 1;
	channelName = this->getArg();
	newTopic = this->getArg();
	if (channelName.empty() == 0)
	{
		status = this->channelFormating(channelName);
		if (status == SUCCESS && channelName.empty() == 0)
			status = this->topic_channel(client,
				channelName, newTopic, topicHaveArg, channels);
		this->errorMessage(status, client);
	}
	else
		this->errorMessage(ERR_NEEDMOREPARAMS, client);
	return (0);
}

int	ChannelCommand::execute(Client *client,
	std::map<std::string, Channel *>& channels)
{
	std::string			buffer;

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
	return (0);
}

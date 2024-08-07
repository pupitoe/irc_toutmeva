/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/07 17:01:20 by ggiboury         ###   ########.fr       */
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
		this->errorMessage(status, client, channel);
		i++;
		buffer_user_name = getPart(user_name, i);
	}
	if (user_name.empty())
		this->errorMessage(ERR_NEEDMOREPARAMS, client, channel);
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
		this->errorMessage(ERR_NEEDMOREPARAMS, client, channelName);
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
		this->errorMessage(ERR_NEEDMOREPARAMS, client, channelName);
	return (SUCCESS);
}

int	ChannelCommand::execute(Client *client, std::map<std::string,
	Channel *>& channels, std::map<int, Client *>& clientLst)
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
	if (buffer == "INVITE")
		return (this->invite(client, channels, clientLst));
	return (0);
}

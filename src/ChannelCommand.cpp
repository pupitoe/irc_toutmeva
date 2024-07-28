/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/27 17:28:27 by tlassere         ###   ########.fr       */
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
	std::map<std::string, Channel *>& channels, std::stringstream& ss)
{
	int			status;
	size_t		i;
	std::string	channel;
	std::string	user_name;
	std::string	comment;
	std::string	buffer_user_name;

	ss >> channel;
	ss >> user_name;
	ss >> comment;
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
	std::map<std::string, Channel *>& channels)
{
	int		status;

	status = ERR_NOSUCHCHANNEL;
	if (channelExist(channelName, channels) == true)
		status = channels[channelName]->topic(user_rqts, newTopic);
	return (status);
}

int	ChannelCommand::topic(Client *client,
	std::map<std::string, Channel *>& channels, std::stringstream& ss)
{
	int			status;
	std::string	channelName;
	std::string	newTopic;

	ss >> channelName;
	ss >> newTopic;
	if (channelName.empty() == 0)
	{
		status = this->channelFormating(channelName);
		if (status == SUCCESS && channelName.empty() == 0)
			status = this->topic_channel(client,
				channelName, newTopic, channels);
		this->errorMessage(status, client);
	}
	else
		this->errorMessage(ERR_NEEDMOREPARAMS, client);
	return (0);
}

int	ChannelCommand::execute(Client *client,
	std::map<std::string, Channel *>& channels)
{
	std::stringstream	ss(this->_msg);
	std::string			buffer;

	ss >> buffer;
	if (buffer == "JOIN")
		return (this->join(client, channels, ss));
	if (buffer == "PART")
		return (this->part(client, channels, ss));
	if (buffer == "KICK")
		return (this->kick(client, channels, ss));
	if (buffer == "TOPIC")
		return (this->topic(client, channels, ss));
	return (0);
}

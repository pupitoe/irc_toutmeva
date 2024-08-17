/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommandJoinPart.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:24:14 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/28 19:18:05tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

std::string	ChannelCommand::getPart(std::string str, size_t pos)
{
	std::string ret;
	size_t		count;

	count = 0;
	while (count < pos && str.find_first_of(',') < str.length())
	{
		str = str.c_str() + str.find_first_of(',') + 1;
		count++;
	}
	if (count == pos)
		ret = str.substr(0, (str.find_first_of(',') < str.length()?
			str.find_first_of(','): str.length()));
	return (ret);
}

int	ChannelCommand::channelFormating(std::string const& name)
{
	int	status;

	status = ERR_NOSUCHCHANNEL;
	if (name.length() > 0 &&  name[0] == '#' && name.find_first_of(' ')
		> name.length())
		status = SUCCESS;
	return (status);
}

int	ChannelCommand::joinChannel(Client* user_rqts,
	std::string const& channelName, std::map<std::string, Channel *>& channels,
	std::string const& key)
{
	int		status;
	Channel	*buffer;

	status = SUCCESS;
	if (channelExist(channelName, channels) == false)
	{
		status = FAIL;
		buffer = new (std::nothrow) Channel(channelName);
		if (buffer)
		{
			try
			{
				channels.insert(std::pair<std::string,
					Channel *>(channelName, buffer));
				status = SUCCESS;
			}
			catch(const std::exception& e)
			{
				delete buffer;
			}
		}
	}
	else
		buffer = channels[channelName];
	if (status == SUCCESS)
		status = buffer->join(user_rqts, key);
	return (status);
}

int	ChannelCommand::join(Client *client,
	std::map<std::string, Channel *>& channels)
{
	int			status;
	size_t		i;
	std::string	channels_name;
	std::string	channels_key;
	std::string	buffer_channel_name;

	channels_name = this->getArg();
	channels_key = this->getArg();
	i = 0;
	buffer_channel_name = getPart(channels_name, i);
	while (i < 100 && buffer_channel_name.empty() == false)
	{
		status = this->channelFormating(buffer_channel_name);
		if (status == SUCCESS)
			status = this->joinChannel(client, buffer_channel_name,
				channels, channels_key);
		this->errorMessage(status, client, channels_name);
		i++;
		buffer_channel_name = getPart(channels_name, i);
	}
	if (channels_name.empty())
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, channels_name);
	return (0);
}

int	ChannelCommand::part(Client *client,
	std::map<std::string, Channel *>& channels)
{
	int			status;
	size_t		i;
	std::string	channels_name;
	std::string	buffer_channel_name;
	std::string	reason;

	channels_name = this->getArg();
	reason = this->getArg();
	i = 0;
	buffer_channel_name = getPart(channels_name, i);
	while (i < 100 && buffer_channel_name.empty() == false)
	{
		std::cout << getPart(channels_name, i) << std::endl;
		status = this->partChannel(client, buffer_channel_name, channels,
			reason);
		this->errorMessage(status, client, channels_name);
		i++;
		buffer_channel_name = getPart(channels_name, i);
	}
	if (channels_name.empty())
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, channels_name);
	return (0);
}

int	ChannelCommand::partChannel(Client* user_rqts,
	std::string const& channelName, std::map<std::string, Channel *>& channels,
	std::string const& reason)
{
	int		status;

	status = ERR_NOSUCHCHANNEL;
	if (channelExist(channelName, channels) == true)
	{
		status = channels[channelName]->part(user_rqts, reason);
		closeChannel(channelName, channels);
	}
	return (status);
}

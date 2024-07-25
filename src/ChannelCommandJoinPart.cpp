/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommandJoinPart.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:24:14 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/25 22:26:33 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

static std::string getPart(std::string str, size_t pos)
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

bool	ChannelCommand::channelExist(std::string const& channelName,
	std::map<std::string, Channel *>& channels) const
{
	return (channels.find(channelName) != channels.end());
}

int	ChannelCommand::join_channel(Client* user_rqts,
	std::string const& channelName, std::map<std::string, Channel *>& channels)
{
	int		status;
	Channel	*buffer;

	status = SUCCESS;
	if (this->channelExist(channelName, channels) == false)
	{
		buffer = new (std::nothrow) Channel(channelName);
		if (buffer)
			channels.insert(std::pair<std::string,
				Channel *>(channelName, buffer));
		else
			status = FAIL;
	}
	else
		buffer = channels[channelName];
	if (status == SUCCESS)
		status = buffer->join(user_rqts);
	return (status);
}

int	ChannelCommand::join(Client *client,
	std::map<std::string, Channel *>& channels, std::stringstream& ss)
{
	int			status;
	size_t		i;
	std::string	channels_name;
	std::string	channels_key;
	std::string	buffer_channel_name;

	ss >> channels_name;
	ss >> channels_key;
	i = 0;
	buffer_channel_name = getPart(channels_name, i);
	while (i < 100 && buffer_channel_name.empty() == 0)
	{
		std::cout << getPart(channels_name, i) << " with key: '" << getPart(channels_key, i) << "'"<< std::endl;
		status = this->channelFormating(buffer_channel_name);
		if (status == SUCCESS)
			status = this->join_channel(client, buffer_channel_name, channels);
		this->errorMessage(status, client);
		i++;
		buffer_channel_name = getPart(channels_name, i);
	}
	if (channels_name.empty())
		this->errorMessage(ERR_NEEDMOREPARAMS, client);
	return (0);
}

int	ChannelCommand::part(Client *client,
	std::map<std::string, Channel *>& channels, std::stringstream& ss)
{
	int			status;
	size_t		i;
	std::string	channels_name;
	std::string	buffer_channel_name;

	ss >> channels_name;
	i = 0;
	buffer_channel_name = getPart(channels_name, i);
	while (i < 100 && buffer_channel_name.empty() == 0)
	{
		std::cout << getPart(channels_name, i) << std::endl;
		status = this->part_channel(client, buffer_channel_name, channels);
		this->errorMessage(status, client);
		i++;
		buffer_channel_name = getPart(channels_name, i);
	}
	if (channels_name.empty())
		this->errorMessage(ERR_NEEDMOREPARAMS, client);
	return (0);
}

int	ChannelCommand::part_channel(Client* user_rqts,
	std::string const& channelName, std::map<std::string, Channel *>& channels)
{
	int		status;

	status = FAIL;
	if (this->channelExist(channelName, channels) == true)
		status = channels[channelName]->part(user_rqts);
	else
		user_rqts->addRPLBuffer(":403 " + user_rqts->getNickName()
			+ " " + channelName + " :No such channel\n");
	return (status);
}

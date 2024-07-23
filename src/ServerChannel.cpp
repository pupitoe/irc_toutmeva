/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 22:09:08 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/23 23:46:47 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool	Server::channelExist(std::string const& channelName) const
{
	bool	status;

	status = false;
	if (this->_channels.find(channelName) != this->_channels.end())
		status = true;
	return (status);
}

int	Server::join_channel(Client* user_rqts, std::string const& channelName)
{
	int		status;
	Channel	*buffer;

	status = SUCCESS;
	if (this->channelExist(channelName) == false)
	{
		buffer = new (std::nothrow) Channel(channelName);
		if (buffer)
			this->_channels.insert(std::pair<std::string,
				Channel *>(channelName, buffer));
		else
			status = FAIL;
	}
	else
		buffer = this->_channels[channelName];
	if (status == SUCCESS)
		status = buffer->join(user_rqts);
	return (status);
}

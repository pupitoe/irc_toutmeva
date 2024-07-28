/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommandMode.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 19:42:04 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/28 15:37:02 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

# define MODE_CHANNEL 0
# define MODE_FLAGS 1

int	ChannelCommand::modeFlags(Client *client,
	std::map<std::string, Channel *>& channels,
	std::string *arg, std::stringstream& ss)
{
//	size_t			i;
//	unsigned int	mode;

//	i = 0;
//	mode = 0;
//	while (flags.empty() == 0 && flags[i])
//	{
//		if (flags[i] == '+')
//			mode = (mode & 15U) | (1 << 9);
//		else if (flags[i] == '-')
//			mode = (mode & 15U) | (1 << 10);
//		else
					
//		i++;
//	}
	(void)client;
	(void)channels;
	(void)arg;
	(void)ss;
	return (SUCCESS);
}

int	ChannelCommand::mode(Client *client,
	std::map<std::string, Channel *>& channels, std::stringstream& ss)
{
	std::string		arg[2];
	int				status;
	
	ss >> arg[MODE_CHANNEL];
	ss >> arg[MODE_FLAGS];
	status = ERR_NEEDMOREPARAMS;
	if (arg[MODE_CHANNEL].empty() == 0)
	{
		status = ERR_NOSUCHCHANNEL;
		if (this->channelFormating(arg[MODE_CHANNEL]) == SUCCESS
			&& channelExist(arg[MODE_CHANNEL], channels) == SUCCESS)
		{
			if (arg[MODE_FLAGS].empty() == 0)
				status = channels[arg[MODE_CHANNEL]]->mode(client);
			else
			{
				status = SUCCESS;
				this->modeFlags(client, channels, arg, ss);
			}
		}
	}
	this->errorMessage(status, client);
	return (0);
}

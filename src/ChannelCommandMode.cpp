/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommandMode.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 19:42:04 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/28 22:13:45 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

# define MODE_CHANNEL 0
# define MODE_FLAGS 1
# define RESERV_BITS 15U
# define FLAG_USED 1
# define FLAG_NO_USED 2

int	ChannelCommand::modeFlagsUse(Client *client, Channel *channel, int mode,
	std::string& arg)
{	
	int	ret;
	int	status;

	ret = FLAG_USED;
	status = 0;
	switch (mode & RESERV_BITS)
	{
		case 'l':
			break;
		case 'i':
			ret = FLAG_NO_USED;
			break;
		case 'o':
			break;
		case 't':
			ret = FLAG_NO_USED;
			break;
		case 'k':
			break;
		default:
			client->addRPLBuffer("tu es une merde\n");
			break;
	}
	(void)client;
	(void)channel;
	(void)mode;
	(void)arg;
	if (status)
		return (status);
	return (ret);
}

int	ChannelCommand::modeFlags(Client *client, Channel *channel,
	std::string& flagLst)
{
	std::string		argFlag;
	unsigned int	mode;
	size_t			i;
	int 			retFalg;

	i = 0;
	mode = 0;
	argFlag = this->getArg();
	retFalg = 0;
	while (!flagLst.empty() && flagLst[i] && retFalg != ERR_CHANOPRIVSNEEDED)
	{
		mode &= ~RESERV_BITS;
		mode |= flagLst[i];
		if (flagLst[i] == '+' || flagLst[i] == '-')
			mode = (1 << 9) + ((flagLst[i] == '-')? 1: 0);
		if (mode & RESERV_BITS)
		{
			retFalg = this->modeFlagsUse(client, channel, mode, argFlag); 
			if (retFalg == FLAG_USED)
				argFlag = this->getArg();
		}
		i++;
	}
	return (SUCCESS);
}

int	ChannelCommand::mode(Client *client,
	std::map<std::string, Channel *>& channels)
{
	std::string		arg[2];
	int				status;
	
	arg[MODE_CHANNEL] = this->getArg();
	arg[MODE_FLAGS] = this->getArg();
	status = ERR_NEEDMOREPARAMS;
	if (arg[MODE_CHANNEL].empty() == 0)
	{
		status = ERR_NOSUCHCHANNEL;
		if (this->channelFormating(arg[MODE_CHANNEL]) == SUCCESS
			&& channelExist(arg[MODE_CHANNEL], channels))
		{
			if (arg[MODE_FLAGS].empty())
				status = channels[arg[MODE_CHANNEL]]->mode(client);
			else
				status = this->modeFlags(client, channels[arg[MODE_CHANNEL]],
					arg[MODE_FLAGS]);
		}
	}
	this->errorMessage(status, client);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommandMode.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 19:42:04 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/04 23:07:27by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

int	ChannelCommand::modeFlagsUse(Client *client, Channel *channel, int mode,
	std::string& arg)
{	
	int		ret;
	int		status;
	bool	signe;

	status = 0;
	signe = ((mode & MODE_SIGNE_BIT) > 0);
	ret = FLAG_USED;
	switch (mode & RESERV_BITS)
	{
		case 'l':
			status = channel->mode_l(client, signe, arg);
			ret = (signe)? FLAG_USED: FLAG_NO_USED;
			break;
		case 'i':
			status = channel->mode_i(client, signe);
			ret = FLAG_NO_USED;
			break;
		case 'o':
			status = channel->mode_o(client, signe, arg);
			break;
		case 't':
			status = channel->mode_t(client, signe);
			ret = FLAG_NO_USED;
			break;
		case 'k':
			status = channel->mode_k(client, signe, arg);
			ret = (signe)? FLAG_USED: FLAG_NO_USED;
			break;
		default:
			status = channel->mode_other(client, mode & RESERV_BITS);
			ret = FLAG_NO_USED;
			break;
	}
	if (status == ERR_CHANOPRIVSNEEDED)
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
		mode |= (unsigned int)flagLst[i];
		if (flagLst[i] == '+' || flagLst[i] == '-')
			mode = MODE_SIGNE_BIT << ((flagLst[i] == '-')? 1: 0);
		if ((mode & RESERV_BITS) && (mode & ~RESERV_BITS))
		{
			retFalg = this->modeFlagsUse(client, channel, mode, argFlag); 
			if (retFalg == FLAG_USED)
				argFlag = this->getArg();
		}
		i++;
	}
	return (SUCCESS);
}

int	ChannelCommand::modePreParserCondition(Channel *channelUse, size_t &usedArg,
	int &signe, int caracter) const
{
	int	retCar;

	retCar = '\0';
	if (caracter == '+')
		signe = MORE;
	else if (caracter == '-')
		signe = LESS;
	if (std::strchr("lko", caracter))
	{
		if (signe == LESS || usedArg < this->_args.size())
			retCar = caracter;
		if (caracter == MORE)
			usedArg++;
	}
	else if (std::strchr("ti", caracter))
	{
		if ((caracter == 't' && signe != channelUse->mode_t_signe())
			|| (caracter == 'i' && signe != channelUse->mode_i_signe()))
			retCar = caracter;
	}
	else
		retCar = caracter;
	return (retCar);
}

std::string ChannelCommand::modePreParser(Channel *channelUse)	
{
	size_t					i;
	int						buffer_caracter;
	t_modePreParserUtils	utils;

	i = 0;
	utils.retFlags = "";
	utils.oldFlags = this->getArg();
	utils.signe = 2;
	utils.usedArg = 0;
	while (utils.oldFlags[i])
	{
		buffer_caracter = this->modePreParserCondition(channelUse,
			utils.usedArg, utils.signe, utils.oldFlags[i]);
		if (buffer_caracter)
			utils.retFlags += buffer_caracter;
		i++;
	}
	if (utils.retFlags.find_first_not_of("+-") >= utils.retFlags.length())
		utils.retFlags.erase();
	return (utils.retFlags);
}

int	ChannelCommand::mode(Client *client,
	std::map<std::string, Channel *>& channels)
{
	std::string		arg[2];
	int				status;
	
	arg[MODE_CHANNEL] = this->getArg();
	status = std::atoi(ERR_NEEDMOREPARAMS);
	if (arg[MODE_CHANNEL].empty() == false)
	{
		status = ERR_NOSUCHCHANNEL;
		if (this->channelFormating(arg[MODE_CHANNEL]) == SUCCESS
			&& channelExist(arg[MODE_CHANNEL], channels))
		{
			arg[MODE_FLAGS] = this->modePreParser(channels[arg[MODE_CHANNEL]]);
			if (arg[MODE_FLAGS].empty())
				status = channels[arg[MODE_CHANNEL]]->mode(client);
			else
				status = this->modeFlags(client, channels[arg[MODE_CHANNEL]],
					arg[MODE_FLAGS]);
		}
		else if (arg[MODE_CHANNEL][0] != '#')
			status = SUCCESS;
	}
	this->errorMessage(status, client, arg[MODE_CHANNEL]);
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 22:03:03 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommand.hpp>

ChannelCommand::ChannelCommand(std::string msg) : Command(msg) {
	//TODO Verifier ici les cmmd;
	this->_type = CHANNEL;
	std::cout << "channel creation" << std::endl;
}

ChannelCommand::~ChannelCommand(void) {
	
}

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

int ChannelCommand::execute(int socket) {
	(void) socket;
	return (0);
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

int	ChannelCommand::join(Client *client,
	std::map<std::string, Channel *>& channels, std::stringstream& ss)
{
	int			status;
	size_t		i;
	std::string	channels_name;
	std::string	channels_key;

	ss >> channels_name;
	ss >> channels_key;
	i = 0;
	while (i < 100 && getPart(channels_name, i).empty() == 0)
	{
		std::cout << getPart(channels_name, i) << " with key: '" << getPart(channels_key, i) << "'"<< std::endl;
		status = this->channelFormating(getPart(channels_name, i));
		if (status == SUCCESS)
			client->addRPLBuffer("holla le tu as rejoin un super serveur\n");
		this->errorMessage(status, client);
		i++;
	}
	if (channels_name.empty())
		this->errorMessage(ERR_NEEDMOREPARAMS, client);
	(void)channels;
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
	(void)client;
	(void)channels;
	return (0);
}

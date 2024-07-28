/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:49 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/28 15:38:58 by ggiboury         ###   ########.fr       */
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


int ChannelCommand::execute(int socket) {
	(void) socket;
	return (0);
}

int	ChannelCommand::join(Client *client,
	std::map<std::string, Channel *>& channels, std::stringstream& ss)
{
	size_t		i;
	std::string	channels_name;
	std::string	channels_key;

	ss >> channels_name;
	ss >> channels_key;
	i = 0;
	while (i < 100 && getPart(channels_name, i).empty() == 0)
	{
		std::cout << getPart(channels_name, i) << " with key: '" << getPart(channels_key, i) << "'"<< std::endl;
		
		i++;
	}
	(void)channels;
	(void)client;
	return (0);
}

int	ChannelCommand::execute(Client *client,
	std::map<std::string, Channel *>& channels)
{
	std::stringstream	ss(this->_args.front());
	std::string			buffer;

	ss >> buffer;
	if (buffer == "JOIN")
		return (this->join(client, channels, ss));
	(void)client;
	(void)channels;
	return (0);
}

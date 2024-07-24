/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:47:12 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/24 17:08:57 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel(std::string const& name): _name(name)
{
	this->_super_user_set = false;
}

Channel::~Channel(void)
{
}

int	Channel::part(Client *client_rqst)
{
	std::list<Client *>::iterator	buffer;

	buffer = std::find(this->_client.begin(), this->_client.end(), client_rqst);
	if (buffer == this->_client.end())
		return (ECHAN_NOT_REGISTERED);
	if (*buffer == this->_super_user)
		this->_super_user = NULL;
	this->_client.erase(buffer);
	return (GOOD_PART);
}

int	Channel::join(Client *client_rqst)
{
	if (std::find(this->_client.begin(), this->_client.end(), client_rqst)
		!= this->_client.end())
		return (ECHAN_ALREADY_REGISTERED);
	if (this->_super_user_set == false)
		this->_super_user = client_rqst;
	this->_client.push_back(client_rqst);
	return (GOOD_REGISTER);
}

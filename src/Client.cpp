/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:37:27 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 15:39:19 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client(void)
{
	this->_terminate_connection = 0;
}

Client::~Client(void)
{
}

bool	Client::getTerminate(void) const
{
	return (this->_terminate_connection);
}
void	Client::changeTerminate(void)
{
	this->_terminate_connection = !this->_terminate_connection;
}

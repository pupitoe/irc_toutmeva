/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 17:01:14 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConnexionCommand.hpp>
#include <iostream>

static void	test_password(const char *str) {
	if (str == NULL || *str == 0){
		throw (IRCError::NeedMoreParams());
	}
}

ConnexionCommand::ConnexionCommand(std::string msg)
	throw (IRCError::NeedMoreParams) : Command(msg) {
	if (!msg.compare(0, 5, "PASS ", 5)){
		test_password(msg.c_str() + 5);
	}
}

ConnexionCommand::~ConnexionCommand(void) {
	
}

int	ConnexionCommand::execute(int socket) {
	(void) socket;
	return (0);
}

int	ConnexionCommand::execute(Client *, std::map<std::string, Channel *>&)
{
	return (0);
}

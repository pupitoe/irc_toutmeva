/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 21:50:39 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConnexionCommand.hpp>
#include <iostream>

static void	test_password(const char *str) {
	std::cout << "test" << std::endl;
	if (str == NULL || *str == 0){
		throw (IRCError(ERR_NEEDMOREPARAMS));
	}
}

ConnexionCommand::ConnexionCommand(std::string msg)
	throw (IRCError) : Command(msg) {
	this->_type = CONNEXION;
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

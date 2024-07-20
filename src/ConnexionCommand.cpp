/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/20 18:08:08 by ggiboury         ###   ########.fr       */
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
	throw (IRCError) : Command(msg) {
	//TODO Verifier ici les cmmd;
	if (msg.compare(0, 5, "PASS ", 5)){
		test_password(msg.c_str() + 5);
	}
}
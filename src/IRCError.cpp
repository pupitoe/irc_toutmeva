/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/09 15:49:46 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRCError.hpp>

IRCError::IRCError(int err) : _err(err) {
	
}

IRCError::IRCError(int err, std::string str) : _err(err), _str(str) {
	
}

IRCError::~IRCError(void) throw() {

}

IRCError::IRCError(IRCError const &ref) {
	_err = ref.getErr();
}

int IRCError::getErr(void) const {
	return (_err);
}

std::string IRCError::getError(void) const {
	return (this->_gen_reply());
}

const char  *IRCError::what(void) const throw() {
	return ("wip");
}

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick)
{
	client->addRPLBuffer(":401 " + client->getNickName() +
		" " + nick + " :No such nick\n");
}

std::string	IRCError::_gen_reply(void) const {
	if (_err == ERR_NEEDMOREPARAMS) {
		return (_str + " " +_str2 + " :Not enough parameters\n");
	}
	else if (_err == ERR_PASSWDMISMATCH) {
		return (_str + " :Password incorrect\n");
	}
	return ("Unknown error\n");
}

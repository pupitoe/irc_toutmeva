/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/07 17:09:49 by ggiboury         ###   ########.fr       */
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


std::string	IRCError::_gen_reply(void) const {
	if (_err == ERR_NEEDMOREPARAMS) {
		return (_str + " " +_str2 + " :Not enough parameters");
	}
	return ("Unknown error");
}
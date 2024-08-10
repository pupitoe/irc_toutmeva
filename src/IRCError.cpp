/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/10 16:48:29 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRCError.hpp>

IRCError::IRCError(std::string err) : _err(err) {
	
}

IRCError::IRCError(std::string err, std::string str) : _err(err), _str(str) {
	
}

IRCError::~IRCError(void) throw() {

}

IRCError::IRCError(IRCError const &ref) {
	_err = ref.getErr();
}

std::string	IRCError::getErr(void) const {
	return (_err);
}

std::string IRCError::getReply(void) const {
	return (this->_gen_reply());
}

const char  *IRCError::what(void) const throw() {
	return ("wip");
}

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick)
{
	client->addRPLBuffer("401\n" + client->getNickName() +
		" " + nick + " :No such nick\n");
}

std::string	IRCError::_gen_reply(void) const {
	std::string res(_err);
	res.append("\n");
	if (_err == ERR_NEEDMOREPARAMS) {
		res.append(_str);
		res.append(" ");
		res.append(_str2);
		res.append(" :Not enough parameters");
	}
	else if (_err == ERR_PASSWDMISMATCH) {
		res.append(_str);
		res.append(" :Password incorrect\n");
	}
	return (res);
}

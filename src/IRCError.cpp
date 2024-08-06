/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/06 16:26:40 by tlassere         ###   ########.fr       */
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

const char  *IRCError::what(void) const throw() {
    return ("wip");
}

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick)
{
	client->addRPLBuffer(":401 " + client->getNickName() +
		" " + nick + " :No such nick\n");
}

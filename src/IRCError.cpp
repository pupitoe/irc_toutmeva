/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/15 10:33:35 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRCError.hpp>

IRCError::IRCError(std::string err,
	std::string str,
	std::string str2) : _err(err), _str(str), _str2(str2)
{}

IRCError::~IRCError(void) throw()
{}

IRCError::IRCError(IRCError const &ref)
{
	_err = ref.getErr();
}

std::string	IRCError::getErr(void) const
{
	return (_err);
}

std::string IRCError::getReply(void) const
{
	return (this->_gen_reply());
}

const char  *IRCError::what(void) const throw()
{
	return ("wip");
}

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick)
{
	client->addRPLBuffer(":irctoutmevas 401 " + client->getNickName() +
		" " + nick + " :No such nick\n");
}

std::string	IRCError::_gen_reply(void) const
{
	std::string	res(":irctoutmevas ");

	res.append(_err);
	res.append(" ");
	if (_err == ERR_UNKNOWNERROR)
	{
		res.append("to write\n");
	}
	else if (_err == ERR_ERRONEUSNICKNAME)
	{
		res.append(_str);
		res.append(" ");
		res.append(_str2);
		res.append(" :Erroneus nickname\n");
	}
	else if (_err == ERR_NEEDMOREPARAMS)
	{
		res.append(_str);
		res.append(" ");
		res.append(_str2);
		res.append(" :Not enough parameters\n");
	}
	else if (_err == ERR_PASSWDMISMATCH)
	{
		res.append(_str);
		res.append(" :Password incorrect\n");
	}
	else if (_err == ERR_NICKNAMEINUSE)
	{
		res.append(_str);
		res.append(" ");
		res.append(_str2);
		res.append(" :Nickname is already in use\n");
	}
	else
	{
		res.append(" :error not implemented\n");
	}
	return (res);
}

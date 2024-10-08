/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/17 21:56:04 by ggiboury         ###   ########.fr       */
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
	return (_str.c_str());
}

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick)
{
	client->addRPLBuffer(":" + (std::string)SERVERNAME + " 401 "
		+ client->getNickName() + " " + nick + " :No such nick\n");
}

std::string	IRCError::_gen_reply(void) const
{
	std::string	res(":" + (std::string)SERVERNAME + " ");

	res.append(_err);
	res.append(" ");
	if (_err == ERR_UNKNOWNERROR)
		res.append(_str + " " + _str2 + " :Unexpected behaviour");
	else if (_err == ERR_NOSUCHNICK)
		res.append(_str + " " + _str2 + " :No such nick/channel");
	else if (_err == ERR_INPUTTOOLONG)
		res.append(_str + " :Input line was too long.");
	else if (_err == ERR_UNKNOWNCOMMAND)
		res.append(_str + " " + _str2 + " :Unknown command");
	else if (_err == ERR_ERRONEUSNICKNAME)
		res.append(_str + " " + _str2 + " :Erroneus nickname");
	else if (_err == ERR_NICKNAMEINUSE)
		res.append(" * " + _str + " :Nickname is already in use");
	else if (_err == ERR_NOTREGISTERED)
		res.append(_str + " :You have not registered");
	else if (_err == ERR_NEEDMOREPARAMS)
		res.append(_str + " " + _str2 + " :Not enough parameters");
	else if (_err == ERR_ALREADYREGISTERED)
		res.append(_str + " :You may not reregister");
	else if (_err == ERR_PASSWDMISMATCH)
		res.append(_str + " :Password incorrect");
	else
		res.append(" :error not implemented");
	res.append("\r\n");
	return (res);
}

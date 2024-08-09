/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/09 10:00:31 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConnexionCommand.hpp>
#include <iostream>

static void	test_password(std::list<std::string> args) {
	if (args.empty() || args.size() < 2) {
		throw (IRCError(ERR_NEEDMOREPARAMS, args.front()));
	}
}

static void test_nickname(std::list<std::string> args) {
	if (args.empty() || args.size() < 2)
		throw (IRCError(ERR_NEEDMOREPARAMS, args.front()));
}

static void test_username(std::list<std::string> args) {
	if (args.empty() || args.size() < 4)
		throw (IRCError(ERR_NEEDMOREPARAMS, args.front()));
}

int	ConnexionCommand::_exec_pass(Client &c) {
	_args.pop_front();
	std::string &pass = *_args.begin();
	if (pass != _password)
		throw (IRCError(ERR_PASSWDMISMATCH));
	c.changeStatus(CS_SETPASS);
	return (0);
}

int	ConnexionCommand::_exec_nick(Client &c) {
	(void) c;
	return (0);
}

int	ConnexionCommand::_exec_user(Client &c) {
	(void) c;
	return (0);
}

ConnexionCommand::ConnexionCommand(std::string msg, std::string password)
	throw (IRCError) : Command(msg), _password(password) {
	this->_type = CONNEXION;
	if (!_args.front().compare(0, 4, "PASS", 4)) {
		test_password(_args);
	}
	else if (!_args.front().compare(0, 4, "NICK", 4)) {
		test_nickname(_args);
	}
	else if (!_args.front().compare(0, 4, "USER", 4)) {
		test_username(_args);
	}
}

ConnexionCommand::~ConnexionCommand(void) {
	
}

int	ConnexionCommand::execute(Client &client) {
	(void) client;
	if (!_args.front().compare(0, 4, "PASS", 4))
		return (_exec_pass(client));
	else if (!_args.front().compare(0, 4, "NICK", 4))
		return (_exec_nick(client));
	else if (!_args.front().compare(0, 4, "USER", 4))
		return (_exec_user(client));
	return (0);
}

int	ConnexionCommand::execute(Client *, std::map<std::string, Channel *>&,
	std::map<int, Client *>&)
{
	
	return (0);
}

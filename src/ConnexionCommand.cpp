/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/09 11:05:01 by ggiboury         ###   ########.fr       */
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
	//Verifications a terminer
	c.changeStatus(CS_SETPASS);
	return (0);
}

static void	registration(Client &c) {
	// RPL WELCOME
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :Welcome to the ");
	c.addRPLBuffer("TMP");
	c.addRPLBuffer(" Network ");
	c.addRPLBuffer(c.getNickName()); // Modulable comme on le souhaite

	//RPL YOUR HOST

}

int	ConnexionCommand::_exec_nick(Client &c) {
	_args.pop_front();
	c.setNickName(_args.front());

	if (c.getStatusClient() == CS_CONNECTED) {
		; // changing nickname;
	}
	else if (c.getStatusClient() == CS_FINISH_REGISTER)	
		registration(c);
	return (0);
}

int	ConnexionCommand::_exec_user(Client &c) {
	_args.pop_front();
	c.setUserName(_args.front());
	_args.pop_front();
	_args.pop_front();
	c.setUserFullName(_args.front());
	if (c.getStatusClient() == CS_FINISH_REGISTER)	
		registration(c);
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

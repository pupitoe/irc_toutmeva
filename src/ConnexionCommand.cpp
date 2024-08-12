/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/12 14:32:00 by ggiboury         ###   ########.fr       */
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
	c.addRPLBuffer(":irctoutmevas 001 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :Welcome to the ");
	c.addRPLBuffer("ft_irc");
	c.addRPLBuffer(" Network ");
	c.addRPLBuffer(c.getNickName()); // Modulable comme on le souhaite
	c.addRPLBuffer("\n");

	//RPL YOUR HOST
	c.addRPLBuffer(":irctoutmevas 002 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :Your host is absent, running version 1.2.3.4\n");

	// RPL CREATED
	c.addRPLBuffer(":irctoutmevas 003 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :This server was created Mon, 31 Jul 4159 26:53:58 UTC\n"); //date to include

	// RPL MYINFO
	c.addRPLBuffer(":irctoutmevas 004 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :absent 1.2.3.4");
	c.addRPLBuffer(" :itkol");
	c.addRPLBuffer("\n"); //date to include

	// RPL ISUPPORT
	c.addRPLBuffer(":irctoutmevas 005 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :MODES");
	c.addRPLBuffer(" :are supported by this server\n");

	//RPL MOTD
	c.addRPLBuffer(":irctoutmevas 375 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :- Message of the day -\n");	
	
	c.addRPLBuffer("Good morning.\n");
	c.addRPLBuffer(":irctoutmevas 376 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :End of MOTD command\n");
}

int	ConnexionCommand::_exec_nick(Client &c) {
	_args.pop_front();
	c.setNickName(_args.front());
	c.changeStatus(CS_SETNICKNAME);
	if (c.getStatusClient() == CS_CONNECTED) {
		; // changing nickname;
	}
	else if (c.getStatusClient() == CS_FINISH_REGISTER)	
		registration(c);
	return (0);
}

// https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3
int	ConnexionCommand::_exec_user(Client &c) {
	_args.pop_front();
	c.setUserName(_args.front());
	_args.pop_front();
	c.setHostName(_args.front());
	_args.pop_front();
	c.setServerName(_args.front());
	_args.pop_front();
	c.setRealName(_args.front());
	c.changeStatus(CS_SETUSER);
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

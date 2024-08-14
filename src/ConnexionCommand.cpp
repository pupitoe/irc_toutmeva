/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/14 11:18:50 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConnexionCommand.hpp>
#include <iostream>

static void	test_password(std::list<std::string> args)
{
	if (args.empty() || args.size() < 2) {
		throw (IRCError(ERR_NEEDMOREPARAMS, args.front()));
	}
}

static void test_nickname(std::list<std::string> args,
	std::map<int, Client *> clientList)
{
	std::map<int, Client *>::iterator	it = clientList.begin();
	std::map<int, Client *>::iterator	ite = clientList.end();
	
	if (args.empty() || args.size() < 2)
		throw (IRCError(ERR_NEEDMOREPARAMS, "client"));

	// Verification of uniqueness
	while (it != ite)
	{
		if (args.front() == it->second->getNickName())
			throw (IRCError(ERR_NICKNAMEINUSE, "client"));
		it++;
	}
}

static void test_username(std::list<std::string> args)
{
	if (args.empty() || args.size() < 5)
		throw (IRCError(ERR_NEEDMOREPARAMS, "client"));
}

int	ConnexionCommand::_exec_pass(Client &c)
{
	if (c.getStatusClient() == CS_SETUSER || c.getStatusClient() == CS_SETNICKNAME)
	{
		c.removeStatus(CS_SETUSER);
		c.removeStatus(CS_SETNICKNAME);
		throw (IRCError(ERR_PASSWDMISMATCH, c.getNickName()));
	}
	_args.pop_front();
	std::string &pass = *_args.begin();
	if (pass != _password)
	{
		c.removeStatus(CS_SETPASS);
		return (0);
	}
	//Verifications a terminer
	c.addStatus(CS_SETPASS);
	return (0);
}

static void	addMOTD(Client &c)
{
	c.addRPLBuffer(":");
	c.addRPLBuffer(SERVERNAME);
	c.addRPLBuffer(" 375 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :- ");
	c.addRPLBuffer(SERVERNAME);
	c.addRPLBuffer(" Message of the day -\n");

	c.addRPLBuffer(":");
	c.addRPLBuffer(SERVERNAME);
	c.addRPLBuffer(" 372 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :- Bienvenue sur ft_webserv x)\n");

	c.addRPLBuffer(":");
	c.addRPLBuffer(SERVERNAME);
	c.addRPLBuffer(" 376 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :End of /MOTD command\n");
}

void	ConnexionCommand::_registration(Client &c) const
{
	if (c.getNickName().find(' ') != std::string::npos
		|| c.getNickName().find('#') != std::string::npos
		|| c.getNickName().find(':') != std::string::npos)
	{
		c.removeStatus(CS_SETNICKNAME);
		c.setHostName("");
		throw (IRCError(ERR_ERRONEUSNICKNAME, "*", "*"));
	}

	if (!(c.getStatusClient() & CS_SETPASS))
	{
		c.removeStatus(CS_FINISH_REGISTER);
		throw IRCError(ERR_PASSWDMISMATCH, c.getNickName());
	}
	c.addStatus(CS_CONNECTED);
	// RPL WELCOME
	c.addRPLBuffer(":");
	c.addRPLBuffer(SERVERNAME);
	c.addRPLBuffer(" 001");
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
	c.addRPLBuffer(" :This server was created Mon, 31 Jul 4159 26:53:58 UTC\n");

	// RPL MYINFO
	c.addRPLBuffer(":irctoutmevas 004 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :absent 1.2.3.4");
	c.addRPLBuffer(" :itkol");
	c.addRPLBuffer("\n");

	// RPL ISUPPORT
	c.addRPLBuffer(":irctoutmevas 005 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :MODES");
	c.addRPLBuffer(" :are supported by this server\n");

	//RPL MOTD
	addMOTD(c);
}

int	ConnexionCommand::_exec_nick(Client &c)
{
	_args.pop_front();
	c.setNickName(_args.front());
	c.addStatus(CS_SETNICKNAME);
	if (c.getStatusClient() == CS_CONNECTED)
		; // changing nickname, condition written for clarity;
	else if (c.getStatusClient() == CS_FINISH_REGISTER
		|| c.getStatusClient() == (CS_FINISH_REGISTER | CS_SETPASS))
		_registration(c);
	return (0);
}

// https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3
int	ConnexionCommand::_exec_user(Client &c)
{
	_args.pop_front();
	c.setUserName(_args.front());
	_args.pop_front();
	c.setHostName(_args.front());
	_args.pop_front();
	c.setServerName(_args.front());
	_args.pop_front();
	c.setRealName(_args.front());
	c.addStatus(CS_SETUSER);
	if (c.getStatusClient() == CS_FINISH_REGISTER
		|| c.getStatusClient() == (CS_FINISH_REGISTER | CS_SETPASS))	
		_registration(c);
	return (0);
}

ConnexionCommand::ConnexionCommand(std::string msg,
	const std::string password,
	const std::map<int, Client *> clientList)
	throw (IRCError) : Command(msg), _password(password)
{
	this->_type = CONNEXION;
	if (!_args.front().compare(0, 4, "PASS", 4)) {
		test_password(_args);
	}
	else if (!_args.front().compare(0, 4, "NICK", 4)) {
		test_nickname(_args, clientList);
	}
	else if (!_args.front().compare(0, 4, "USER", 4)) {
		test_username(_args);
	}
}

ConnexionCommand::~ConnexionCommand(void)
{}

int	ConnexionCommand::execute(Client &client)
{
	if (!_args.front().compare(0, 4, "PASS", 4))
		return (_exec_pass(client));
	else if (!_args.front().compare(0, 4, "NICK", 4))
		return (_exec_nick(client));
	else if (!_args.front().compare(0, 4, "USER", 4))
		return (_exec_user(client));
	return (0);
}

int	ConnexionCommand::execute(Client *,
	std::map<std::string, Channel *>&,
	std::map<int, Client *>&)
{
	
	return (0);
}

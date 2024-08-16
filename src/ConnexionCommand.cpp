/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:12:08 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/16 23:15:24 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConnexionCommand.hpp>
#include <iostream>

void	ConnexionCommand::_testPassword(void) const
{
	if (_args.empty() || _args.size() < 2) {
		throw (IRCError(ERR_NEEDMOREPARAMS, _args.front()));
	}
}

void ConnexionCommand::_testNickname(std::map<int, Client *> clientList)
{
	std::string							username;
	std::map<int, Client *>::iterator	it = clientList.begin();
	std::map<int, Client *>::iterator	ite = clientList.end();
	
	if (_args.empty() || _args.size() < 2)
		throw (IRCError(ERR_NEEDMOREPARAMS, "client", "NICK"));

	// Verification of uniqueness
	_args.pop_front();
	username = _args.front();
	_args.insert(_args.begin(), "NICK");
	
	while (it != ite)
	{
		if (!username.compare(it->second->getNickName()))
			throw (IRCError(ERR_NICKNAMEINUSE, username));
		it++;
	}
}

void ConnexionCommand::_testUsername(Client &c) const
{
	if (_args.empty() || _args.size() < 5)
		throw (IRCError(ERR_NEEDMOREPARAMS, c.getNickName(), "USER"));
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
		// c.setHostName(""); //todo,
		throw (IRCError(ERR_ERRONEUSNICKNAME, "*", "*"));
	}
	
	if (!(c.getStatusClient() & CS_SETPASS))
	{
		c.removeStatus(CS_FINISH_REGISTER);
		throw IRCError(ERR_PASSWDMISMATCH, c.getNickName());
	}
	c.addStatus(CS_CONNECTED);
	c.removeStatus(CS_FINISH_REGISTER);
	// RPL WELCOME
	c.addRPLBuffer(":");
	c.addRPLBuffer(SERVERNAME);
	c.addRPLBuffer(" 001 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :Welcome to the ");
	c.addRPLBuffer("ft_irc");
	c.addRPLBuffer(" Network ");
	c.addRPLBuffer(c.getNickName()); // Modulable comme on le souhaite
	c.addRPLBuffer("\n");

	//RPL YOUR HOST
	c.addRPLBuffer(":" + (std::string)SERVERNAME + " 002 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :Your host is absent, running version 1.2.3.4\n");

	// RPL CREATED
	c.addRPLBuffer(":" + (std::string)SERVERNAME + " 003 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :This server was created Mon, 31 Jul 4159 26:53:58 UTC\n");

	// RPL MYINFO
	c.addRPLBuffer(":" + (std::string)SERVERNAME + " 004 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :absent 1.2.3.4");
	c.addRPLBuffer(" :itkol");
	c.addRPLBuffer("\n");

	// RPL ISUPPORT
	c.addRPLBuffer(":" + (std::string)SERVERNAME + " 005 ");
	c.addRPLBuffer(c.getNickName());
	c.addRPLBuffer(" :MODES");
	c.addRPLBuffer(" :are supported by this server\n");

	//RPL MOTD
	addMOTD(c);
}

int	ConnexionCommand::_execPass(Client &c)
{
	if (c.getStatusClient() == CS_CONNECTED)
		throw (IRCError(ERR_ALREADYREGISTERED, c.getNickName()));
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

void	changeNickname(Client &c, std::string new_nickname)
{
	c.addRPLBuffer(":");
	c.addRPLBuffer(c.getInfo());
	c.addRPLBuffer(" NICK ");
	c.addRPLBuffer(new_nickname);
	c.setNickName(new_nickname);
}

int	ConnexionCommand::_execNick(Client &c)
{
	_args.pop_front();
	c.addStatus(CS_SETNICKNAME);
	if (c.getStatusClient() & CS_CONNECTED)
		changeNickname(c, _args.front());
	else if (c.getStatusClient() & CS_FINISH_REGISTER)
		c.setNickName(_args.front());
	if (c.getStatusClient() == CS_FINISH_REGISTER
		|| c.getStatusClient() == (CS_FINISH_REGISTER | CS_SETPASS))
		_registration(c);
	return (0);
}

// https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3
int	ConnexionCommand::_execUser(Client &c)
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

int	ConnexionCommand::_execQuit(Client &c)
{
	_args.pop_front();
	c.terminateConnection();
	return (0);
}

ConnexionCommand::ConnexionCommand(std::string msg,
	const std::string password,
	const std::map<int, Client *> clientList,
	Client &c)
	throw (IRCError)
	: Command(msg), _password(password)
{
	this->_type = CONNEXION;
	if (!_args.front().compare(0, 4, "PASS", 4))
		_testPassword();
	else if (!_args.front().compare(0, 4, "NICK", 4))
		_testNickname(clientList);
	else if (!_args.front().compare(0, 4, "USER", 4))
		_testUsername(c);
}

int	ConnexionCommand::ping(Client *client)
{
	std::string	msg;

	this->getArg();
	if (this->_args.size() > 0)
	{
		msg = this->getArg();
		if (msg.empty())
			this->ERR_NOORIGIN_MSG(client);
		else
			client->addRPLBuffer("PONG " + (std::string)SERVERNAME
				+ " :" + msg + "\n");
	}
	else
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, "");
	return (SUCCESS);
}

int	ConnexionCommand::pong(Client *client)
{
	std::string	msg;

	this->getArg();
	if (this->_args.size() > 0)
	{
		msg = this->getArg();
		if (msg.empty())
			this->ERR_NOORIGIN_MSG(client);
		else if (client->getSendPing() && msg == PING_WORD)
		{
			client->setLastPing(std::time(NULL));
			client->setSendPing(false);
		}
	}
	else
		this->errorMessage(std::atoi(ERR_NEEDMOREPARAMS), client, "");
	return (SUCCESS);
}

void	 ConnexionCommand::ERR_NOORIGIN_MSG(Client *client)
{
	client->addRPLBuffer(":" + (std::string)SERVERNAME + " 409 " + 
		client->getNickName() + " :No origin specified\n");
}

ConnexionCommand::~ConnexionCommand(void)
{}

int	ConnexionCommand::execute(Client &client)
{
	if (!_args.front().compare("PASS"))
		return (_execPass(client));
	else if (!_args.front().compare("NICK"))
		return (_execNick(client));
	else if (!_args.front().compare("USER"))
		return (_execUser(client));
	else if (!_args.front().compare("QUIT"))
		return (_execQuit(client));
	else if (_args.front() == "PING")
		return (this->ping(&client));
	else if (_args.front() == "PONG")
		return (this->pong(&client));
	return (0);
}

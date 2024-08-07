/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/06 16:29:43 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Command.hpp>
#include <iostream>

#include <sstream>
#include <algorithm>

static bool	isCommand(std::string str) {
	unsigned int	i = 0;
	while (str[i]) {
		if (str[i] < 65 || str[i] > 90)
			return (false);
		i++;
	}
	return (true);
}

/**
 * Here's the parsing for our IRC:
 * 	CMD *PARAM
 * 
 * The spaces can be 1 or more spaces (character " ")
 * 
 * CMD := <letters>
 * 
 * PARAM := <optional ":"><letters>
 * 
 * parsing step :
 * 1. Ignore first word if its a command
 * 2. For every word after that
 *	2a. If it begins with ":" , then the rest of msg is final param
 * 	2b. Else, parse normally.
 * */
Command::Command(std::string msg) throw (Command::UnrecognizedType, IRCError) {
	std::string			word;
	std::stringstream	str(msg);

	if (msg.length() > MESSAGES_LIMIT)
		throw (IRCError(ERR_INPUTTOOLONG));
		
	//Command
	str >> word;
	for (unsigned int i = 0 ; word[i] != 0 ; i++) {
		word[i] = std::toupper(word[i]);
	}
	if (!isCommand(word))
		throw (IRCError(ERR_UNKNOWNERROR)); // ??? Or 472
	_args.push_back(word);
	
	//Parameters
	while (str >> word) {
		if (word[0] == ':') {
			if (str.tellg() > 0 && (size_t)str.tellg() < str.str().length())
				word.append(str.str().substr(str.tellg(), str.str().size()));
			word.erase(0, 1);
			_args.push_back(word);
			break ;
		}
		_args.push_back(word);
	}
	this->_type = EMPTY;
	if (this->_args.size())
		this->_command_name = this->_args.front();
	this->_nb_arg = this->_args.size();
}

Command::Command(Command const &ref) : _args(ref.getArgs()),
	_type(ref.getType()) {

}

Command::~Command(void) {

}

std::list<std::string>	Command::getArgs(void) const {
	return (_args); // A modifier, copie partielle
}

cmd_type	Command::getType(void) const {
	return (_type);
}

const char	*Command::UnrecognizedType::what(void) const throw() {
	return ("Type of command received invalid.");
};

std::ostream	&operator<<(std::ostream &out, Command const &c) {
	return (out << c.getArgs().front());
}

void	Command::errorMessage(int error, Client *client,
	std::string const& channelName)
{
	switch (error)
	{
		case ERR_NEEDMOREPARAMS:
			client->addRPLBuffer(":461 " + client->getNickName()
				+ " " + this->_command_name + " :Not enough parameters\n");
			break;
		case ERR_NOSUCHCHANNEL:
			client->addRPLBuffer(":403 " + client->getNickName()
				+ " " + channelName + " :No such channel\n");
			break;
		default:
			break;
	}
}

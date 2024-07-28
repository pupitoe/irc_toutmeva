/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/28 12:17:35 by ggiboury         ###   ########.fr       */
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
 * CMD := <letters> | <3 digit number>
 * 
 * PARAM := <optional ":"><letters>
 * 
 * parsing step :
 * 1. Ignore first word
 * 2. For every word after that
 *	2a. If it begins with ":" , then the rest of msg is final param
 * 	2b. Else, parse normally.
 * */
Command::Command(std::string msg) throw (Command::UnrecognizedType, IRCError) :
	_msg(msg) {
	if (msg.length() > MESSAGES_LIMIT)
		throw (IRCError(ERR_INPUTTOOLONG));
	
	for (unsigned int i = 0 ; msg[i] != 0 ; i++) {
		_msg[i] = std::toupper(_msg[i]);
	}

	std::stringstream str(_msg);
	std::string word;

	str >> word;

	if (!isCommand(word))
		throw (IRCError(ERR_UNKNOWNERROR)); // ??? Or 472
	while (str >> word){
		std::cout << word << std::endl;
	}
	this->_type = EMPTY;
}

Command::Command(Command const &ref) : _msg(ref.getMsg()),
	_type(ref.getType()) {

}

Command::~Command(void) {

}

std::string	Command::getMsg(void) const {
	return (_msg);	
}

enum type	Command::getType(void) const {
	return (_type);
}

const char	*Command::UnrecognizedType::what(void) const throw() {
	return ("Type of command received invalid.");
};

std::ostream	&operator<<(std::ostream &out, Command const &c) {
	std::string str = c.getMsg().substr(0, 5);
	return (out << str);
}

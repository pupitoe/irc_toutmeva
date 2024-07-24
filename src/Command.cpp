/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 21:49:48 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Command.hpp>
#include <iostream>

#include <sstream>

Command::Command(std::string msg) throw (Command::UnrecognizedType, IRCError) :
	_msg(msg) {
	//parsing
	if (msg.length() < MESSAGES_LIMIT)
		throw (IRCError(ERR_INPUTTOOLONG));
	std::cout << "Debut" <<std::endl;
	
	std::stringstream test(msg);
	std::string word;
	while (test >> word){
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/17 16:30:16 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Command.hpp>
#include <iostream>

static enum type guessType(std::string msg) {
	if (msg.compare(0, 5, "PASS ", 5) || msg.compare(0, 5, "NICK ")
		|| msg.compare(0, 5, "USER ") || msg.compare(0, 4, "CAP "))
		return (CONNEXION);
	else if (msg.compare(0, 5, "JOIN ", 5) || msg.compare(0, 5, "PART  ", 5)
		|| msg.compare(0, 5, "TOPIC ", 6) || msg.compare(0, 5, "NAMES  ", 6)
		|| msg.compare(0, 5, "LIST ", 5) || msg.compare(0, 5, "INVITE ", 7)
		|| msg.compare(0, 5, "KICK ", 5))
		return (CHANNEL);
	else if (msg.empty())
		return (EMPTY);
	return (ERR);
}

Command::Command(std::string msg) throw (Command::UnrecognizedType){
	_type = guessType(msg);
	if (_type == ERR)
		throw (Command::UnrecognizedType());
}


Command::Command(Command const &ref) : _type(ref.getType()), _msg(ref.getMsg()){
		
}

Command::~Command(void){

}

std::string	Command::getMsg(void) const {
	return (_msg);	
}


enum type   Command::getType(void) const {
	return (_type);
}

const char  *Command::UnrecognizedType::what(void) const throw() {
	return ("Type of command received invalid.");
};

std::ostream   &operator<<(std::ostream &out, Command const &c){
	std::string str;
	switch (c.getType()){
		case ERR:
			str.append("Error");
			break;
		case CONNEXION:
			str.append("Connexion");
			break;
		case CHANNEL:
			str.append("Channel");
			break;
		case EMPTY:
			str.append("Channel");
			break;
		default :
			str.append("Empty");
			break ;
	}
	str.append(" command.");
	return (out << str);
}
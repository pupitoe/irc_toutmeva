/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/22 15:26:07 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Command.hpp>
#include <iostream>

Command::Command(std::string msg) throw (Command::UnrecognizedType) : _msg(msg) {
	_msg = msg;
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
	std::string str = c.getMsg().substr(0, 5);
	return (out << str);
}

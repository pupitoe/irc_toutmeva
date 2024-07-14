/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/14 16:20:04 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Command.hpp>

static enum type guessType(std::string msg) {
    if (msg.compare(0, 5, "PASS ", 5))
        return (CONNEXION);
    else if (msg.compare(0, 5, "JOIN ", 5))
        return (CHANNEL);
    return (ERR);
}

Command::Command(std::string msg) throw (Command::UnrecognizedType){
    _type = guessType(msg);
    if (_type == ERR)
        throw (Command::UnrecognizedType());
}


Command::~Command(void){

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
        default :
            str.append("Unexpected");
            break ;
    }
    str.append(" command.");
    return (out << str);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/13 21:09:13 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Command.hpp>

static enum type guessType(std::string msg) {
    (void) msg;
    return (ERR);
}

Command::Command(char *msg) throw (Command::UnrecognizedType){
    _type = guessType(msg);
    if (_type == ERR)
        throw (Command::UnrecognizedType());

}



enum type   Command::getType(void) const {
    return (_type);
}


const char  *Command::UnrecognizedType::what(void) const throw() {
    return ("Type of command received invalid.");
};
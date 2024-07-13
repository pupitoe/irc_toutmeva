/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:02:01 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/13 20:49:27 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRCParser.hpp>

IRCParser::IRCParser(void) {
	
}

IRCParser::~IRCParser(void) {
	
}


bool    IRCParser::commandIsValid(char *str) {
    if (str == NULL)
        return (false);
    return (true);
}

void    IRCParser::parse(std::string str){
    // write()
    (void) str;
}
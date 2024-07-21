/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/21 17:28:37 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRCError.hpp>

IRCError::~IRCError(void) throw() {
    
}


const char    *IRCError::NeedMoreParams::what(void) const throw() {
    return ("<client> <command> : NotEnough parameters");
}
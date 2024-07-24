/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:20:00 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 21:55:00 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRCError.hpp>

IRCError::IRCError(int err) : _err(err){
    
}

IRCError::~IRCError(void) throw() {

}

IRCError::IRCError(IRCError const &ref) {
    _err = ref.getErr();
}

int IRCError::getErr(void) const {
    return (_err);
}

const char  *IRCError::what(void) const throw(){
    return ("ahahahahahahah");
}








//These functions are only used for debugging (and may be used for logs)

// const char    *IRCError::NeedMoreParams::what(void) const throw() {
//     return ("<client> <command> : NotEnough parameters");
// }

// const char    *IRCError::InputTooLong::what(void) const throw() {
//     return ("The limit for messages is MESSAGE_LIMIT");
// }

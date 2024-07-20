/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:14:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/20 15:53:57 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCERROR_HPP
# define IRCERROR_HPP

# include <exception>
// Don't forget to see if everything is correctly used.
class IRCError : public std::exception {

    public :
        virtual ~IRCError(void) throw(); // Verifier ce que c'est !

    class NeedMoreParams {
        const char  *what(void) const throw();
    };
    
    class Passwdmismatch {
        const char  *what(void) const throw();
    };
    
    class AlreadyRegistered {
        const char  *what(void) const throw();
    };
};

#endif
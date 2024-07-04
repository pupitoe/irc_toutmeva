/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:13:41 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/04 11:40:13 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSOCKET_HPP
# define IRCSOCKET_HPP

# include <sys/socket.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include <cstring>
# include <exception>

# include "irc_tout_me_va.hpp" // SUCCESS/FAIL VAR
# include "ft_error.hpp"

# define SIZE_QUEUE 100

class   IRCSocket{
    private:
		int _socket_fd;

        IRCSocket(void);
        
    public:
        IRCSocket(int port) throw (IRCSocket::FailedToCreate, IRCSocket::PortInvalid);
        ~IRCSocket(void);

        int getSocketFd(void) const;

    // Exceptions
    class FailedToCreate : public std::exception{
        const char  *what(void) const throw();  
    };
    class PortInvalid : public std::exception{
        const char  *what(void) const throw();  
    };
};

#endif
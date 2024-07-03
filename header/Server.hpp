/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:09:14 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/03 17:20:13 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <utility>
# include <unistd.h>
# include <string>

# include "IRCSocket.hpp"
# include "Client.hpp"
# include "irc_tout_me_va.hpp"

# define SIZE_MESSAGE_BUFFER 1024

class IRCSocket;

class	Server
{
	private:
		std::string				_password;
		int						_port;

		IRCSocket				*_socket;
    
    // the complete fds table
		fd_set					_rfds;
		std::map<int, Client *>	_clientList;
		
		int						_status_server;

		void	clientRecvMessage(int const client_fd, Client& client);

	public:
		Server(void);
		Server(char *psw, int port);
		~Server(void);

		fd_set	getFdSet(void) const;
		Client	*getClient(int const fd);

		void	addClient(int const fd);
		void	deletClient(int const fd);

		int		getStatus(void) const;
		
		void	searchClient(void);
		void	clientRecv(void);
		
		void	eraseClient(void);
};

#endif

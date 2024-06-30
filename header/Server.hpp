/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:09:14 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/30 21:10:18 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <map>
# include <utility>
# include <unistd.h>

# include "Client.hpp"
# include "irc_tout_me_va.hpp"
# include "ft_socket.hpp"

# define SIZE_MESSAGE_BUFFER 1024

class	Server
{
	private:
		// the complete fds table
		fd_set					_rfds;
		std::map<int, Client *>	_clientList;
		
		int						_socket_fd;
		int						_status_server;

		void	clientRecvMessage(int const client_fd, Client& client);

	public:
		Server(void);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:09:14 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/22 15:44:10 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <utility>
# include <string>

# include "IRCSocket.hpp"
# include "Client.hpp"
# include "irc_tout_me_va.hpp"
# include "IRCSocket.hpp"
# include "Command.hpp"
# include "ConnexionCommand.hpp"
# include "ChannelCommand.hpp"
# include "IRCError.hpp"

# define SIZE_MESSAGE_BUFFER 1024

class	Server
{
	private:
		std::string				_password;

		IRCSocket				_socket;
    		
		// the complete fds table
		fd_set					_rfds;
		std::map<int, Client *>	_clientList;

		fd_set					_rfds_read;
		fd_set					_rfds_write;
		fd_set					_rfds_error;
		
		int						_status_server;

		void	clientRecvMessage(int const client_fd, Client& client);

		void	addClient(int const fd);
		void	deletClient(int const fd);

		void	searchClient(void);
		void	clientRecv(void);
		void	parseInput(void);
		void	executeRequests(void);
		void	eraseClient(void);

		void	useSelect(void);

	public:
		Server(void);
		Server(char *psw, int port);
		~Server(void);

		fd_set	getFdSet(void) const;
		Client	*getClient(int const fd);

		int		getStatus(void) const;

		void	execut(void);
		void	parse(std::string cmd, Client &c);
};

#endif

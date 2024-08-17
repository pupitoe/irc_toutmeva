/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:09:14 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 18:06:33 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <string>
# include <ctime>

# include "irc_tout_me_va.hpp"
# include "IRCSocket.hpp"
# include "IRCError.hpp"
# include "Client.hpp"
# include "Bot.hpp"
# include "ConnexionCommand.hpp"
# include "ChannelCommand.hpp"
# include "Channel.hpp"

# define SIZE_MESSAGE_BUFFER 1024
# define MAX_TIME_PING 60

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

		int						_status;

		std::map<std::string, Channel *>	_channels;

		void	clientRecvMessage(int const client_fd, Client& client);
		void	clientSendMessage(int const client_fd, Client& client);

		void	addClient(int const fd);
		void	deleteClient(int const fd, bool serverTerminate);

		void	searchClient(void);
		void	clientRecv(void);
		void	clientSend(void);
		void	parseInput(void);
		void	eraseClient(void);

		void	useSelect(void);

		void	userPing(void);

		void	addBot(void);

		void	sendBot(Client *bot);

	public:
		Server(char *psw, int port);
		~Server(void);

		fd_set	getFdSet(void) const;
		Client	*getClient(int const fd);

		int		getStatus(void) const;

		void	execute(void);
		void	parse(std::string cmd, Client &c);
};

#endif

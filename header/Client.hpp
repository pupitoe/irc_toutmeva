/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:13:37 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/21 18:23:32 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <queue>
# include <string>

# include <Command.hpp>

enum	CStatus
{
	CS_NOTHING = 0,
	CS_TERMINATED = 1,
	CS_SETNICKNAME = 1 << 1,
	CS_SETUSER = 1 << 2,
	CS_SETPASS = 1 << 3,
	CS_FINISH_REGISTER = CS_SETNICKNAME | CS_SETUSER | CS_SETPASS
};

class Command;

class	Client
{
	private:
		int const	_client_fd;
		int			_status_connection;

		std::string	_nickName;
		std::string	_userName;
		std::string	_hostName;
		std::string _userNameFull;
		std::string	_serverName;

		std::string	_bufferCommand;
		
		std::queue<Command*> requests;
		
	public:
		Client(int const client_fd);
		~Client(void);
		
		int		getStatusClient(void) const;
		void	terminateConnection(void);

		void	setNickName(std::string const& str);
		void	setUserName(std::string const& str);
		void	setHostName(std::string const& str);
		void	setUserFullName(std::string const& str);

		void	setServerName(std::string const& str);

		std::string const&	getCommandBuffer(void) const;
		std::string			getCommand(void);
		bool				getCommandValible(void);
		void				addCommandBuffer(char const *cmd);

		void				addRequest(Command *c);
		bool				hasRequest(void);
		Command				*nextRequest(void);
};

#endif

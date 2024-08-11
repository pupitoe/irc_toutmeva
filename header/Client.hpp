/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:13:37 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/11 23:09:20 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <queue>
# include <map>
# include <string>
# include <ctime>

enum	CStatus
{
	CS_NOTHING = 0,
	CS_TERMINATED = 1,
	CS_SETNICKNAME = 1 << 1,
	CS_SETUSER = 1 << 2,
	CS_SETPASS = 1 << 3,
	CS_FINISH_REGISTER = CS_SETNICKNAME | CS_SETUSER | CS_SETPASS,
	CS_CONNECTED = 1 << 4
};

class	Client
{
	protected:
		int const	_client_fd;
		int			_status_connection;

		std::string	_nickName;
		std::string	_userName;
		std::string	_hostName;
		std::string _userNameFull;
		std::string	_serverName;

		std::string	_bufferCommand;
		std::string	_bufferRPL;

		time_t		_lastPing;

		bool		_sendPing;
		bool		_bot;

	public:
		Client(int const client_fd);
		virtual ~Client(void);
		
		int		getStatusClient(void) const;
		int		getFd(void) const;
		void	terminateConnection(void);

		void	setNickName(std::string const& str);
		void	setUserName(std::string const& str);
		void	setHostName(std::string const& str);
		void	setUserFullName(std::string const& str);
		void	changeStatus(enum CStatus new_status);

		void	setServerName(std::string const& str);

		std::string const&	getCommandBuffer(void) const;
		std::string			getCommand(void);
		bool				getCommandValible(void);
		void				addCommandBuffer(char const *cmd);

		void				addRPLBuffer(std::string const& rpl);
		std::string const&	getRPLBuffer(void) const;
		std::string			getRPL(void);

		std::string const&	getNickName(void) const;

		time_t				lastPingTime(time_t ctime) const;
		void				setLastPing(time_t ctime);
		void				setSendPing(bool set);
		bool				getSendPing(void) const;
		bool				getBot(void) const;
};

Client	*getClientMap(std::string const& nickName,
	std::map<int, Client*>clientsLst);

#endif

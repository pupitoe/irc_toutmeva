/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:13:37 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 21:29:35 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

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
	CS_FINISH_REGISTER = CS_SETNICKNAME | CS_SETUSER,
	CS_CONNECTED = 1 << 4
};

class	Client
{
	protected:
		int			_status_connection;

		std::string	_nickName;
		std::string	_userName;
		std::string	_hostName;
		std::string _userRealName;
		std::string	_serverName;

		std::string	_bufferCommand;
		std::string	_bufferRPL;

		time_t		_lastPing;

		bool		_sendPing;
		bool		_bot;

	public:
		Client(void);
		virtual ~Client(void);
		
		int		getStatus(void) const;
		void	terminateConnection(void);

		void	setNickName(std::string const& str);
		void	setUserName(std::string const& str);
		void	setHostName(std::string const& str);
		void	setRealName(std::string const& str);
		void	addStatus(enum CStatus status);
		void	removeStatus(enum CStatus status);

		void	setServerName(std::string const& str);

		std::string const&	getCommandBuffer(void) const;
		std::string			getCommand(void);
		bool				getCommandValided(void);
		void				addCommandBuffer(char const *cmd);
		void				addCommandBuffer(std::string const& cmd);

		void				addRPLBuffer(std::string const& rpl);
		std::string const&	getRPLBuffer(void) const;
		std::string			getRPL(void);

		std::string const&	getNickName(void) const;

		time_t				lastPingTime(time_t ctime) const;
		void				setLastPing(time_t ctime);
		void				setSendPing(bool set);
		bool				getSendPing(void) const;
		bool				getBot(void) const;

		std::string			getInfo(void) const;
};

Client	*getClientMap(std::string const& nickName,
	std::map<int, Client*>clientsLst);

#endif

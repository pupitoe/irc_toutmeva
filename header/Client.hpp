/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:13:37 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/30 17:47:27 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

enum	CStatus
{
	CS_NOTHING = 0,
	CS_TERMINATED = 1
};

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
};

#endif

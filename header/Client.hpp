/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:13:37 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 20:07:39 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class	Client
{
	private:
		bool		_terminate_connection;

		std::string	_nickName;
		std::string	_userName;
		std::string	_hostName;
		std::string _userNameFull;
		std::string	_serverName;

	public:
		Client(void);
		~Client(void);
		
		bool	getTerminate(void) const;
		void	changeTerminate(void);

		void	setNickName(std::string const& str);
		void	setUserName(std::string const& str);
		void	setHostName(std::string const& str);
		void	setUserFullName(std::string const& str);

		void	setServerName(std::string const& str);
};

#endif

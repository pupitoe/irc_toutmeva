/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:09:14 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 15:39:47 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <map>
# include <utility>
# include <unistd.h>
# include "Client.hpp"

class	Server
{
	private:
		fd_set					_rfds;
		std::map<int, Client>	_clientList;

	public:
		Server(void);
		~Server(void);

		fd_set	getFdSet(void) const;
		Client&	getClient(int const fd);

		void	addClient(int const fd);
		void	deletClient(int const fd);
		
};

#endif

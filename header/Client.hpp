/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:13:37 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 15:38:13 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class	Client
{
	private:
		bool	_terminate_connection;

	public:
		Client(void);
		~Client(void);
		
		bool	getTerminate(void) const;
		void	changeTerminate(void);
};

#endif

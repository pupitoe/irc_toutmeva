/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:15 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 15:09:30 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Client.hpp"
# include <iostream>

class	Bot: public Client 
{
	public:
		Bot(int const client_fd);
		~Bot(void);

		void	RPL(std::string const str);
};

#endif

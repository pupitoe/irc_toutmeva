/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:15 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/13 21:16:12 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Client.hpp"
# include <iostream>
# include <Morfi.hpp>

# define MAX_MORFI_GAMES 5

class	Bot: public Client 
{
	private:
		std::map<std::string, Morfi*> _morfiGames;

	public:
		Bot(int const client_fd);
		~Bot(void);

		void		RPL(std::string const str);
		std::string	game(std::string const& params);
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:15 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/15 21:35:01 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Client.hpp"
# include <iostream>
# include <Morfi.hpp>

# define MAX_MORFI_GAMES 5
# define GAME_MAX_TIME 600 // (10 minutes)

class	Bot: public Client 
{
	private:
		std::map<std::string, Morfi*> _morfiGames;

	public:
		Bot(int const client_fd);
		~Bot(void);

		void		RPL(std::string const str);

		std::map<std::string, Morfi*>&	getMorfi(void);
		int		getMorfiStat(std::string const& target);
		bool	creatGame(std::string const& target,
			std::string const gameName);
		void	deleteGame(std::string const& gameName);
		void	checkGames(void);
};

#endif

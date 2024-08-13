/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:57:15 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/13 22:53:45 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Client.hpp"
# include <iostream>
# include <Morfi.hpp>

# define MAX_MORFI_GAMES 5

enum MORFI_STAT
{
	MO_NOTHING = 0,
	MO_NOT_CREAT,
	MO_BOT_ROUND,
	MO_TARGET_ROUND
};

class	Bot: public Client 
{
	private:
		std::map<std::string, Morfi*> _morfiGames;

	public:
		Bot(int const client_fd);
		~Bot(void);

		void		RPL(std::string const str);

		std::map<std::string, Morfi*>&	getMorfi(void);
		int	getMorfiStat(std::string const& target);
		bool	creatGame(std::string const& target,
			std::string const gameName);
};

#endif

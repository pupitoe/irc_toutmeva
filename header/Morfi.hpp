/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Morfi.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:51:51 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/14 22:41:08 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MORFI_HPP
# define MORFI_HPP

# include <string>
# include <cstdlib>
# include <ctime>
# include <cstring>
# include "irc_tout_me_va.hpp"

# define CASE_NO_SET 0
# define CASE_P1 1
# define CASE_P2 2
# define GRID_SIZE 9

# define PLACE(x) (x == CASE_P1)? 'X': '0'

enum MORFI_STAT
{
	MO_NOTHING = 0,
	MO_BOT_ROUND,
	MO_TARGET_ROUND,
	MO_NOT_CREAT
};

class	Morfi
{
	private:
		std::string const	_p1;
		std::string const	_p2;

		int					_roundUser;
		unsigned int		_roundStep;
		int					_grid[GRID_SIZE];

	public:
		Morfi(std::string const& p1, std::string const& p2);
		~Morfi(void);

		int	getRound(void) const;

		std::string	const&	getUserRound(void) const;
		std::string	const&	getP1(void) const;
		std::string	const&	getP2(void) const;
		int const			*getGrid(void) const;
		
		int					place(int width, int height);
};

#endif

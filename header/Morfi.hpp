/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Morfi.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:51:51 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/14 21:49:04 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MORFI_HPP
# define MORFI_HPP

# include <string>
# include <cstdlib>
# include <ctime>
# include <cstring>

# define CASE_NO_SET 0
# define CASE_P1 1
# define CASE_P2 2
# define GRID_SIZE 9

enum MORFI_STAT
{
	MO_NOTHING = 0,
	MO_NOT_CREAT,
	MO_BOT_ROUND,
	MO_TARGET_ROUND
};

class	Morfi
{
	private:
		std::string const	_p1;
		std::string const	_p2;

		int					_round;
		int					_grid[GRID_SIZE];

	public:
		Morfi(std::string const& p1, std::string const& p2);
		~Morfi(void);

		int	getRound(void) const;

		std::string	const&	getUserRound(void) const;
		std::string	const&	getP1(void) const;
		std::string	const&	getP2(void) const;
		int const			*getGrid(void) const;
		
};

#endif

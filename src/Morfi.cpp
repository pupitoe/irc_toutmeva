/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Morfi.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:54:09 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/14 22:50:16 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Morfi.hpp>

Morfi::Morfi(std::string const& p1, std::string const& p2): _p1(p1), _p2(p2)
{
	std::srand(std::time(NULL));
	this->_roundUser = MO_BOT_ROUND + std::rand() % 2;
	std::memset(this->_grid, CASE_NO_SET, sizeof(int) * 9);
	_roundStep = 0;
}

Morfi::~Morfi(void)
{
}

int	Morfi::getRound(void) const
{
	return (this->_roundUser);
}

std::string const&	Morfi::getUserRound(void) const
{
	if (this->_roundUser == MO_BOT_ROUND)
		return (this->_p1);
	return (this->_p2);
}

int const	*Morfi::getGrid(void) const
{
	return (this->_grid);
}

std::string const&	Morfi::getP1(void) const
{
	return (this->_p1);
}

std::string const&	Morfi::getP2(void) const
{
	return (this->_p2);
}

int	Morfi::place(int width, int height)
{
	int	pos;
	int ret;

	pos = -1;
	ret = FAIL;
	if (width >= 0 && width <= 2 && height >= 0 && height <= 2)
	{
		pos = width + height * 3;
		if (!this->_grid[pos])
		{
			this->_grid[pos] = CASE_P1 + this->_roundStep % 2;
			this->_roundStep++;
			this->_roundUser ^= MO_BOT_ROUND | MO_TARGET_ROUND;
			ret = SUCCESS;
		}
	}
	return (ret);
}

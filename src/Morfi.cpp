/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Morfi.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:54:09 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/16 15:35:13 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Morfi.hpp>

Morfi::Morfi(std::string const& p1, std::string const& p2): _p1(p1), _p2(p2)
{
	std::srand(std::time(NULL));
	this->_roundUser = MO_P1 + std::rand() % 2;
	std::memset(this->_grid, CASE_NO_SET, sizeof(int) * 9);
	this->_roundStep = 0;
	this->_lastTime = std::time(NULL);
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
	if (this->_roundUser == MO_P1)
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
			this->_roundUser ^= MO_P1 | MO_P2;
			ret = SUCCESS;
			this->_lastTime = std::time(NULL);
		}
	}
	return (ret);
}

int	Morfi::winner(int caseWin)
{
	unsigned int	i;

	i = 0;
	while (i < GRID_SIZE && _grid[i])
		i++;
	if (i == GRID_SIZE && caseWin == CASE_NO_SET)
		return (MO_NULL);
	else if (caseWin == CASE_P1 || caseWin == CASE_P2)
		return (this->_roundUser ^ (MO_P1 | MO_P2));
	return (MO_NOT_FINISH);
}

int	Morfi::finished(void)
{
	unsigned int	i;
	int				buffer;

	i = 0;
	buffer = CASE_NO_SET;
	while (i < 3 && !buffer)
	{
		if (this->_grid[i] == this->_grid[i + 3]
			&& this->_grid[i] == this->_grid[i + 6])
			buffer = this->_grid[i];
		else if (this->_grid[i * 3] == this->_grid[i * 3 + 1]
			&& this->_grid[i * 3] == this->_grid[i * 3 + 2])
			buffer = this->_grid[i * 3];
		i++;
	}
	if (!buffer && this->_grid[0] == this->_grid[4]
		&& this->_grid[0] == this->_grid[8])
		buffer = this->_grid[0];
	else if (!buffer && this->_grid[2] == this->_grid[4]
		&& this->_grid[2] == this->_grid[6])
		buffer = this->_grid[4];
	return (this->winner(buffer));
}

time_t	Morfi::getLastTime(void) const
{
	return (this->_lastTime);
}

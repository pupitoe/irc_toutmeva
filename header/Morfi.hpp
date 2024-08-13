/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Morfi.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:51:51 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/13 22:55:29 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MORFI_HPP
# define MORFI_HPP

# include <string>

class	Morfi
{
	private:
		std::string const	_p1;
		std::string const	_p2;

	public:
		Morfi(std::string const& p1, std::string const& p2);
		~Morfi(void);
};

#endif

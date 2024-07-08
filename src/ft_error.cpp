/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 01:01:28 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/07 18:20:19 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ft_error.hpp>

void	ft_error_print(int const err)
{
	if (err == E_SO_REUSEADDR)
		std::cout << "setsockopt: SO_REUSEADDR fail" << std::endl;
	else if (err == E_SO_REUSEPORT)
		std::cout << "setsockopt: SO_REUSEPORT fail" << std::endl;
	else if (err == E_TCP_NODELAY)
		std::cout << "setsockopt: TCP_NODELAY fail" << std::endl;
	else if (err == E_BIND)
		std::cout << "bind: fail" << std::endl;
	else if (err == E_LISTEN)
		std::cout << "listen: fail" << std::endl;
}

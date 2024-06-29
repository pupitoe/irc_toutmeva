/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 00:58:28 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 01:03:07 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERROR_HPP
# define FT_ERROR_HPP

# include <iostream>

enum	ERROR_STATUS
{
	E_SO_REUSEADDR = 10,
	E_SO_REUSEPORT,
	E_TCP_NODELAY,
	E_BIND,
	E_LISTEN
};

void	ft_error_print(int const err);

#endif

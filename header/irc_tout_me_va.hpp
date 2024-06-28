/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:00:34 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/28 16:11:41 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# define SUCCESS 0
# define FAIL 1

enum	ERROR_STATUS
{
	E_SO_REUSEADDR = 10,
	E_SO_REUSEPORT,
	E_TCP_NODELAY,
	E_BIND,
	E_LISTEN
};

#endif

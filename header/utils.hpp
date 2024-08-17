/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 18:18:14 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/17 19:33:16 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# define SUCCESS 0
# define FAIL 1

# define MORE 1
# define LESS 0

# define PING_WORD "coucou"
# define BOT_NAME "giveyouup"

# define SERVERNAME "irctoutmevas"

enum	retChannel
{
	ECHAN_ALREADY_REGISTERED = 3,
	ECHAN_NOT_REGISTERED,
	ERR_NOSUCHCHANNEL,
	ERR_BADCHANNELKEY,
	ERR_CHANNELISFULL,
	ERR_INVITEONLYCHAN,
	ERR_CHANOPRIVSNEEDED,

	GOOD_REGISTER,
	GOOD_PART,
	BOT_NOT_PART
};

#endif

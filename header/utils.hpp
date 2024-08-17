/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 18:18:14 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/17 19:05:27 by ggiboury         ###   ########.fr       */
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
	ECHAN_BAD_ALLOC = 2,
	ECHAN_ALREADY_REGISTERED,
	ECHAN_NOT_REGISTERED,
	ERR_NOSUCHCHANNEL,
	ERR_BADCHANNELKEY,
	ERR_CHANNELISFULL,
	ERR_INVITEONLYCHAN,
	ERR_BADCHANMASK,
	ERR_CHANOPRIVSNEEDED,
	RPL_TOPIC,
	RPL_TOPICWHOTIME,
	RPL_NAMREPLY,
	RPL_ENDOFNAMES,

	GOOD_REGISTER,
	GOOD_PART,
	BOT_NOT_PART
};

#endif

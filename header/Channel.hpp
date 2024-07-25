/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:39:58 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/25 12:27:14 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <list>
# include <string>
# include <algorithm>

enum	retChannel
{
	ECHAN_BAD_ALLOC = 2,
	ECHAN_ALREADY_REGISTERED,
	ECHAN_NOT_REGISTERED,
	ERR_NEEDMOREPARAMS,
	ERR_NOSUCHCHANNEL,
	ERR_TOOMANYCHANNELS,
	ERR_BADCHANNELKEY,
	ERR_BANNEDFROMCHAN,
	ERR_CHANNELISFULL,
	ERR_INVITEONLYCHAN,
	ERR_BADCHANMASK,
	RPL_TOPIC,
	RPL_TOPICWHOTIME,
	RPL_NAMREPLY,
	RPL_ENDOFNAMES,


	GOOD_REGISTER,
	GOOD_PART
};

class	Channel
{
	private:
		std::string const	_name;
		Client*				_super_user;
		std::list<Client *>	_client;

		int					_super_user_set;
		//std::list<Client *>	_operator;
	
		std::string			_topic;

	public:
		Channel(std::string const& str);
		~Channel(void);

		int	join(Client* client_rqst);
		int	part(Client* client_rqst);
};

#endif

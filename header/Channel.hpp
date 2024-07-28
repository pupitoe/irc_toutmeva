/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:39:58 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/29 01:01:01 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <list>
# include <string>
# include <algorithm>
# include <map>
# include <ctime>
# include <sstream>

# define MODE_ADD 1
# define MODE_REMOVE 0

enum	retChannel
{
	ECHAN_BAD_ALLOC = 2,
	ECHAN_ALREADY_REGISTERED,
	ECHAN_NOT_REGISTERED,
	ERR_NOSUCHCHANNEL,
	ERR_TOOMANYCHANNELS,
	ERR_BADCHANNELKEY,
	ERR_BANNEDFROMCHAN,
	ERR_CHANNELISFULL,
	ERR_INVITEONLYCHAN,
	ERR_BADCHANMASK,
	ERR_CHANOPRIVSNEEDED,
	RPL_TOPIC,
	RPL_TOPICWHOTIME,
	RPL_NAMREPLY,
	RPL_ENDOFNAMES,


	GOOD_REGISTER,
	GOOD_PART
};

enum	userChannelGrade
{
	CH_NO_GRADE = 0,
	CH_USER,
	CH_OPERATOR
};

class	Channel
{
	private:
		std::string const	_name;
		std::list<Client *>	_operators;
		std::list<Client *>	_client;

		int					_super_user_set;

		std::string			_topic_usr;
		std::string			_topic;
		std::string			_creation_time;

		bool				_topic_priv_need;

		int	inLst(Client *client);
		int	inOpLst(Client *client);

		void	RPL_NAMREPLY(Client *client);
		void	RPL_ENDOFNAMES(Client *client);
		void	RPL_CREATIONTIME(Client* client_rqst);

		int	userGrade(std::string const& nickName);

		void	kickActiv(Client* client_rqst, std::string const& userKick,
			std::string const& comment);

		Client	*getClient(std::string const& nickName);
		void	sendAll(std::string const& msg);

		void	topicActiv(Client* client_rqst, std::string const& newTopic,
			int topicHaveArg);
		void	topicChange(Client* client_rqst, std::string const& newTopic);
		void	topicRPL(Client *client_rqst);

	public:
		Channel(std::string const& str);
		~Channel(void);

		int	join(Client* client_rqst);
		int	part(Client* client_rqst, std::string const& reason);
		int	kick(Client* client_rqst, std::string const& userKick,
			std::string const& comment);
		int	topic(Client* client_rqst, std::string const& newTopic,
			int topicHaveArg);
		int	mode(Client* client_rqst);
		int	mode_t(Client* client_rqst, int signe);

		size_t	countClient(void) const;
};

void	closeChannel(std::string const& channelName,
	std::map<std::string, Channel *>& channels);
bool	channelExist(std::string const& channelName,
	std::map<std::string, Channel *>& channels);

#endif

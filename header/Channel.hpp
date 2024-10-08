/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:39:58 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 19:21:45 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <list>
# include <string>
# include <algorithm>
# include <map>
# include <ctime>
# include <sstream>

# include "Client.hpp"
# include "utils.hpp"
# include "IRCError.hpp"

# define MODE_ADD 1
# define MODE_REMOVE 0
# define LIMIT_USER_IN_CHANNEL 4048

typedef	unsigned int	userlimit;

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

		std::list<Client *>	_invite_lst;

		int					_super_user_set;

		std::string			_topic_usr;
		std::string			_topic;
		std::string			_creation_time;

		bool				_topic_priv_need;
		bool				_invite_only;
		userlimit			_limit;
		std::string			_key;

		void	ERR_CHANOPRIVSNEEDED_MSG(Client *client);
		void	ERR_NOTONCHANNEL_MSG(Client *client);
		void	ERR_USERNOTINCHANNEL_MSG(Client *client,
			std::string const& nick);
		void	ERR_USERONCHANNEL_MSG(Client *client, std::string const& nick);
		void	ERR_BAD_JOIN_BOT(Client *client, std::string const& nick,
			std::string const& channel);

		int	inLst(Client *client) const;
		int	inOpLst(Client *client) const;
		int	inInviteList(Client *client) const;

		void	RPL_NAMREPLY(Client *client);
		void	RPL_ENDOFNAMES(Client *client);
		void	RPL_CREATIONTIME(Client* client_rqst);


		void	joinSuperUser(Client* client_rqst);
		int		userGrade(std::string const& nickName);
		int		joinCheck(Client *client_rqst, std::string const& key);

		void	kickActiv(Client* client_rqst, std::string const& userKick,
			std::string const& comment);

		Client	*getClient(std::string const& nickName);
		void	sendAll(std::string const& msg);

		void	topicActiv(Client* client_rqst, std::string const& newTopic,
			int topicHaveArg);
		void	topicChange(Client* client_rqst, std::string const& newTopic);
		void	topicRPL(Client *client_rqst);

		int		modeBasic(bool *modeVar, int signe, char typeMode,
			Client *client_rqst);
		void	sendInviteClient(Client* client_rqst,
			std::string const& userName, std::map<int, Client *>& clientsLst);
		void	RPL_MODE_L(Client *client_rqst);
		void	RPL_JOIN_MSG_ERR(Client *client_rqst, std::string const& error,
			char type);
		int	mode_k_analyse(Client* client_rqst, std::string const& key);

	public:
		Channel(std::string const& str);
		~Channel(void);

		int	join(Client* client_rqst, std::string const& key);
		int	part(Client* client_rqst, std::string const& reason);
		int	kick(Client* client_rqst, std::string const& userKick,
			std::string const& comment);
		int	topic(Client* client_rqst, std::string const& newTopic,
			int topicHaveArg);
		int	invite(Client* client_rqst, std::string const& userName,
			std::map<int, Client *>& clientsLst);

		int	mode(Client* client_rqst);
		int	mode_t(Client* client_rqst, int signe);
		int	mode_o(Client* client_rqst, int signe, std::string const& user);
		int	mode_l(Client* client_rqst, int signe, std::string const& limit);
		int	mode_k(Client* client_rqst, int signe, std::string const& key);
		int	mode_i(Client* client_rqst, int signe);
		int	mode_other(Client* client_rqst, char flag);

		bool mode_t_signe(void) const;
		bool mode_i_signe(void) const;

		size_t	countClient(void) const;
		void	eraseInviteLst(Client *client);
		int		sendMsg(Client* client_rqst,
			std::string const& message, int op);
};

void	closeChannel(std::string const& channelName,
	std::map<std::string, Channel *>& channels);
bool	channelExist(std::string const& channelName,
	std::map<std::string, Channel *>& channels);

void	RPL_PRIVMSG(Client *client_rqst, Client *dest,
	std::string const& message, std::string const& target);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:39:58 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/03 20:15:14 by tlassere         ###   ########.fr       */
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

		std::list<Client *>	_invite_lst;

		int					_super_user_set;

		std::string			_topic_usr;
		std::string			_topic;
		std::string			_creation_time;

		bool				_topic_priv_need;
		bool				_invite_only;

		int	inLst(Client *client) const;
		int	inOpLst(Client *client) const;
		int	inInvitLst(Client *client) const;

		void	RPL_NAMREPLY(Client *client);
		void	RPL_ENDOFNAMES(Client *client);
		void	RPL_CREATIONTIME(Client* client_rqst);


		void	join_super_user(Client* client_rqst);
		int	userGrade(std::string const& nickName);

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
		void	sendInvitClient(Client* client_rqst,
			std::string const& userName, std::map<int, Client *>& clientsLst);

	public:
		Channel(std::string const& str);
		~Channel(void);

		int	join(Client* client_rqst);
		int	part(Client* client_rqst, std::string const& reason,
			bool quitServe);
		int	kick(Client* client_rqst, std::string const& userKick,
			std::string const& comment);
		int	topic(Client* client_rqst, std::string const& newTopic,
			int topicHaveArg);
		int	invite(Client* client_rqst, std::string const& userName,
			std::map<int, Client *>& clientsLst);
		int	mode(Client* client_rqst);
		int	mode_t(Client* client_rqst, int signe);
		int	mode_o(Client* client_rqst, int signe, std::string& user);
		int	mode_i(Client* client_rqst, int signe);

		size_t	countClient(void) const;
		void	eraseInviteLst(Client *client);
};

void	closeChannel(std::string const& channelName,
	std::map<std::string, Channel *>& channels);
bool	channelExist(std::string const& channelName,
	std::map<std::string, Channel *>& channels);

#endif

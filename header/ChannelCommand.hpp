/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/17 19:25:49 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELCOMMAND_HPP
# define CHANNELCOMMAND_HPP

# include <string>
# include <iostream>
# include <cstring>
# include <algorithm>

# include "Command.hpp"
# include "Channel.hpp"
# include "utils.hpp"

# define MODE_CHANNEL 0
# define MODE_FLAGS 1
# define RESERV_BITS 255U
# define FLAG_USED 1
# define FLAG_NO_USED 2
# define MODE_SIGNE_BIT (1 << 9)

typedef struct s_modePreParserUtils
{
	size_t		usedArg;
	int			signe;
	std::string	retFlags;
	std::string	oldFlags;
}	t_modePreParserUtils;

class ChannelCommand : public Command
{
	private:
		int	join(Client *client, std::map<std::string, Channel *>& channels);
		int	part(Client *client, std::map<std::string, Channel *>& channels);
		int	kick(Client *client, std::map<std::string, Channel *>& channels);
		int	topic(Client *client, std::map<std::string, Channel *>& channels);
		int	invite(Client *client, std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst);
		int	mode(Client *client, std::map<std::string, Channel *>& channels);
		int	modeFlags(Client *client, Channel *channel, std::string& flagLst);
		int	modeFlagsUse(Client *client, Channel *channel, int mode,
			std::string& arg);
		int	modePreParserCondition(Channel *channelUse, size_t &usedArg,
			int &signe, int caracter) const;
		int	privmsg(Client *client, std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst);
		int	privmsgExec(Client *client,
			std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst, std::string const& target,
			std::string const& message);
		int	privmsgExecChannel(Client *client,
			std::map<std::string, Channel *>& channels,
			std::string const& target, std::string const& message);
		int	privmsgExecClient(Client *client,
			std::map<int, Client *>& clientLst, std::string const& target,
			std::string const& message);

		std::string	modePreParser(Channel *channelUse);

		int	channelFormating(std::string const& name);

		int	joinChannel(Client* user_rqts, std::string const& channelName,
			std::map<std::string, Channel *>& channels, std::string const& key);
		int	partChannel(Client* user_rqts, std::string const& channelName,
			std::map<std::string, Channel *>& channels,
			std::string const& reason);
		int	kickChannel(Client* user_rqts, std::string const& channelName,
			std::string const& userKick, std::string const& comment,
			std::map<std::string, Channel *>& channels);
		int	topicChannel(Client* user_rqts,
			std::string const& channelName, std::string const& newTopic,
			int topicHaveArg, std::map<std::string, Channel *>& channels);

		std::string getPart(std::string str, size_t pos);

    public :
        ChannelCommand(std::string msg);
        ~ChannelCommand(void);

		int	execute(Client *client, std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst);
};

#endif

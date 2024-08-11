/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/11 22:24:18 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELCOMMAND_HPP
# define CHANNELCOMMAND_HPP

# include <Command.hpp>
# include <string>
# include <sstream>
# include <iostream>
# include <cstring>
# include "irc_tout_me_va.hpp"

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
		int	privmsg_exec(Client *client,
			std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst, std::string const& target,
			std::string const& message);
		int	privmsg_exec_channel(Client *client,
			std::map<std::string, Channel *>& channels,
			std::string const& target, std::string const& message);
		int	privmsg_exec_client(Client *client,
			std::map<int, Client *>& clientLst, std::string const& target,
			std::string const& message);
		int	ping(Client *client);
		int	pong(Client *client);

		std::string	modePreParser(Channel *channelUse);

		int	channelFormating(std::string const& name);

		int	join_channel(Client* user_rqts, std::string const& channelName,
			std::map<std::string, Channel *>& channels, std::string const& key);
		int	part_channel(Client* user_rqts, std::string const& channelName,
			std::map<std::string, Channel *>& channels,
			std::string const& reason);
		int	kick_channel(Client* user_rqts, std::string const& channelName,
			std::string const& userKick, std::string const& comment,
			std::map<std::string, Channel *>& channels);
		int	topic_channel(Client* user_rqts,
			std::string const& channelName, std::string const& newTopic,
			int topicHaveArg, std::map<std::string, Channel *>& channels);

		std::string getPart(std::string str, size_t pos);

		std::string	getArg(void);
		void	 ERR_NOORIGIN_MSG(Client *client);

    public :
        ChannelCommand(std::string msg);
        ~ChannelCommand(void);

        int execute(Client &client);
		int	execute(Client *client, std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst);
};

#endif

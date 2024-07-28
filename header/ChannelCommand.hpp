/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/28 20:05:33 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELCOMMAND_HPP
# define CHANNELCOMMAND_HPP

# include <Command.hpp>
# include <IRCError.hpp>
# include <string>
# include <sstream>
# include <iostream>
# include "irc_tout_me_va.hpp"

class ChannelCommand : public Command {
	private:
		int	join(Client *client, std::map<std::string, Channel *>& channels);
		int	part(Client *client, std::map<std::string, Channel *>& channels);
		int	kick(Client *client, std::map<std::string, Channel *>& channels);
		int	topic(Client *client, std::map<std::string, Channel *>& channels);
		int	mode(Client *client, std::map<std::string, Channel *>& channels,
			std::stringstream& ss);
		int	modeFlags(Client *client,
			std::map<std::string, Channel *>& channels,
			std::string *arg, std::stringstream& ss);

		int	channelFormating(std::string const& name);
		
		void	errorMessage(int error, Client *);

		int	join_channel(Client* user_rqts, std::string const& channelName,
			std::map<std::string, Channel *>& channels);
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
    public :
        ChannelCommand(std::string msg);
        ~ChannelCommand(void);

        int execute(int socket);
		int	execute(Client *client, std::map<std::string, Channel *>& channels);
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/25 12:14:46 by tlassere         ###   ########.fr       */
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
		int	join(Client *client, std::map<std::string, Channel *>& channels,
			std::stringstream& ss);
		int	channelFormating(std::string const& name);
		
		void	errorMessage(int error, Client *);
		bool	channelExist(std::string const& channelName,
			std::map<std::string, Channel *>& channels) const;

		int	join_channel(Client* user_rqts, std::string const& channelName,
			std::map<std::string, Channel *>& channels);

    public :
        ChannelCommand(std::string msg);
        ~ChannelCommand(void);

        int execute(int socket);
		int	execute(Client *client, std::map<std::string, Channel *>& channels);
};

#endif

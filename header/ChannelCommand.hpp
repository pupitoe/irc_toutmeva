/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 19:04:58 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELCOMMAND_HPP
# define CHANNELCOMMAND_HPP

# include <Command.hpp>
# include <IRCError.hpp>
# include <string>
# include <sstream>
# include <iostream>

class ChannelCommand : public Command {
	private:
		int	join(Client *client, std::map<std::string, Channel *>& channels,
			std::stringstream& ss);

    public :
        ChannelCommand(std::string msg);
        ~ChannelCommand(void);

        int execute(int socket);
		int	execute(Client *client, std::map<std::string, Channel *>& channels);
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:11:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/22 15:25:24 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELCOMMAND_HPP
# define CHANNELCOMMAND_HPP

# include <Command.hpp>
# include <IRCError.hpp>
# include <string>

class ChannelCommand : public Command {

    public :
        ChannelCommand(std::string msg);
        ~ChannelCommand(void);

        int execute(int socket);
};

#endif

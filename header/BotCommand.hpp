/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:40:17 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 00:46:44 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTCOMMAND_HPP
# define BOTCOMMAND_HPP

# include <Command.hpp>
# include <string>
# include <iostream>
# include "irc_tout_me_va.hpp"

class BotCommand: public Command
{
	public:
    	BotCommand(std::string msg);
        ~BotCommand(void);

        int execute(Client &client);
		int	execute(Client *client, std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst);
};

#endif

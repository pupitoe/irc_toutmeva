/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:40:17 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 01:06:21 by tlassere         ###   ########.fr       */
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
	private:
		std::string	_user;
		std::string	_cmd;

		std::string	getUserName(std::string const& user) const;

	public:
    	BotCommand(std::string msg);
        ~BotCommand(void);

        int execute(Client &client);
		int	execute(Client *client, std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst);
};

#endif

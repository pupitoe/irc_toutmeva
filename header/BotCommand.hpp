/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:40:17 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 15:11:16 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTCOMMAND_HPP
# define BOTCOMMAND_HPP

# include <Command.hpp>
# include <string>
# include <iostream>
# include "irc_tout_me_va.hpp"

class BotCommand
{
	private:
		std::string				_user;
		std::string				_cmd;
		std::list<std::string>	_args;

		std::string	getUserName(std::string const& user) const;

		std::string	getArg(void);

	public:
    	BotCommand(std::string msg);
        ~BotCommand(void);

        int execute(void);
};

#endif

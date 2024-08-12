/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:40:17 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/12 20:35:56 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTCOMMAND_HPP
# define BOTCOMMAND_HPP

# include <Command.hpp>
# include <string>
# include <iostream>
# include "irc_tout_me_va.hpp"
# include "Bot.hpp"

class BotCommand
{
	private:
		Bot						*_cbot;
		std::string				_user;
		std::string				_cmd;
		std::list<std::string>	_args;

		std::string	getUserName(std::string const& user) const;

		std::string	getArg(void);

		void	sendPrivmsg(std::string const& target, std::string const& msg);
		void	privmsg(void);
		void	invite(void);
		void	badJoinChan(void);
		void	part(void);

	public:
    	BotCommand(std::string msg, Bot *cbot);
        ~BotCommand(void);

        void	execute(void);
};

#endif

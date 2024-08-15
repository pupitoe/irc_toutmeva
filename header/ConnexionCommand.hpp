/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:09:33 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/15 10:10:54 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNEXIONCOMMAND_HPP
# define CONNEXIONCOMMAND_HPP

# include <Command.hpp>
# include <IRCError.hpp>
# include <string>

class ConnexionCommand : public Command {

	private :
		std::string	_password;
		bool		_nick_already_used;

		int _exec_pass(Client &);
		int _exec_nick(Client &);
		int _exec_user(Client &);
		int	_exec_quit(Client &);

		void	_registration(Client &c) const;

	public :
		ConnexionCommand(std::string msg,
			const std::string password,
			const std::map<int, Client *>)
			throw (IRCError);
		~ConnexionCommand(void);

		int execute(Client &client);
		int	execute(Client *client, std::map<std::string,
			Channel *>& channels,
			std::map<int, Client *>& clientLst);
};

#endif

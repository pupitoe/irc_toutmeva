/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:09:33 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/16 17:19:36 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNEXIONCOMMAND_HPP
# define CONNEXIONCOMMAND_HPP

# include <Command.hpp>
# include <IRCError.hpp>
# include <string>

class ConnexionCommand : public Command
{

	private :
		std::string	_password;

		void	_testPassword(void) const;
		void	_testNickname(std::map<int, Client *> clientList);
		void	_testUsername(void) const;

		int _execPass(Client &);
		int _execNick(Client &);
		int _execUser(Client &);
		int	_execQuit(Client &);

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

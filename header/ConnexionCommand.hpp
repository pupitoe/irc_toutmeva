/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:09:33 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/17 19:06:27 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNEXIONCOMMAND_HPP
# define CONNEXIONCOMMAND_HPP

# include <string>
# include <algorithm>

# include "Command.hpp"
# include "Client.hpp"
# include "IRCError.hpp"

class ConnexionCommand : public Command
{
	private :
		std::string	_password;

		void		_testPassword(void) const;
		void		_testNickname(std::map<int, Client *> clientList);
		void		_testUsername(Client &c) const;

		int 		_execPass(Client &);
		int 		_execNick(Client &);
		int			_execUser(Client &);
		int			_execQuit(Client &);
		
		int			ping(Client *client);
		int			pong(Client *client);
		void		ERR_NOORIGIN_MSG(Client *client);

		void		_registration(Client &c) const;

	public :
		ConnexionCommand(std::string msg,
			const std::string password,
			const std::map<int, Client *>,
			Client &c)
			throw (IRCError);
		~ConnexionCommand(void);

		int execute(Client &client);
};

#endif

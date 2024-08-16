/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:09:33 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/16 20:55:57 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNEXIONCOMMAND_HPP
# define CONNEXIONCOMMAND_HPP

# include <Command.hpp>
# include <Client.hpp>
# include <IRCError.hpp>
# include <string>

class ConnexionCommand : public Command
{

	private :
		std::string	_password;

		void	_testPassword(void) const;
		void	_testNickname(std::map<int, Client *> clientList);
		void	_testUsername(Client &c) const;

		int _execPass(Client &);
		int _execNick(Client &);
		int _execUser(Client &);
		int	_execQuit(Client &);
		int	ping(Client *client);
		int	pong(Client *client);
		void	 ERR_NOORIGIN_MSG(Client *client);

		void	_registration(Client &c) const;

	public :
		ConnexionCommand(std::string msg,
			const std::string password,
			const std::map<int, Client *>,
			Client &c)
			throw (IRCError);
		~ConnexionCommand(void);

		int execute(Client &client);
		int	execute(Client *client, std::map<std::string,
			Channel *>& channels,
			std::map<int, Client *>& clientLst);
};

#endif

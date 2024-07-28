/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/28 15:22:15 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

class Command;

# include <string>
# include <exception>
# include <map>

# include <IRCError.hpp>
# include <Channel.hpp>

enum type {
	ERR,
	EMPTY,
	CONNEXION,
	CHANNEL
}	typedef cmd_type;
# define MESSAGES_LIMIT 512
/*
* Polymorphic class
* */
class Command {

	protected :
		std::list<std::string>	_args;
		cmd_type				_type; // Verifier a la fin si c'est toujours utile d'avoir cet attribut

	public :
		Command(std::string msg) 
			throw (Command::UnrecognizedType, IRCError);
		Command(Command const &);
		virtual ~Command(void);

		std::list<std::string>	getArgs(void) const;
		virtual enum type getType(void) const;

		virtual int	execute(int socket) = 0;
		virtual int	execute(Client *client,
			std::map<std::string, Channel *>& channels) = 0;

		class UnrecognizedType : public std::exception {
			const char	*what(void) const throw();
		};
};

std::ostream   &operator<<(std::ostream &, Command const &);

# endif

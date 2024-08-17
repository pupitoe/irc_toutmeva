/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/17 19:31:48 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

class Command;

# include <string>
# include <exception>
# include <sstream>
# include <list>
# include <algorithm>

# include <IRCError.hpp>

# define MESSAGES_LIMIT 512

enum type {
	ERR,
	EMPTY,
	CONNEXION,
	CHANNEL
}	typedef cmd_type;

/*
* Polymorphic class
* */
class Command
{

	protected :
		std::string 			_command_name;
		std::list<std::string>	_args;
		cmd_type				_type; // Verifier a la fin si c'est toujours utile d'avoir cet attribut
		size_t					_nb_arg;

		void	errorMessage(int error, Client *client,
			std::string const& channelName);

		std::string	getArg(void);

	public :
		Command(std::string msg) 
			throw (IRCError);
		Command(Command const &);
		virtual ~Command(void);

		std::list<std::string>	getArgs(void) const;
		virtual enum type getType(void) const;
};

void			ft_split_word(std::string const& msg,
	std::list<std::string>& args);
std::string		ft_getArg(std::list<std::string>& arg);

// std::ostream   &operator<<(std::ostream &, Command const &);

# endif

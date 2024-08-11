/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/12 00:38:41 by tlassere         ###   ########.fr       */
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
		std::string 			_command_name;
		std::list<std::string>	_args;
		cmd_type				_type; // Verifier a la fin si c'est toujours utile d'avoir cet attribut
		size_t					_nb_arg;

		void	errorMessage(int error, Client *client,
			std::string const& channelName);

	public :
		Command(std::string msg) 
			throw (Command::UnrecognizedType, IRCError);
		Command(Command const &);
		virtual ~Command(void);

		std::list<std::string>	getArgs(void) const;
		virtual enum type getType(void) const;

		virtual int	execute(Client &client) = 0;
		virtual int	execute(Client *client,
			std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst) = 0;

		class UnrecognizedType : public std::exception {
			const char	*what(void) const throw();
		};
};

std::ostream   &operator<<(std::ostream &, Command const &);

# endif

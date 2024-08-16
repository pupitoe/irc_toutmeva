/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/16 15:41:58 by ggiboury         ###   ########.fr       */
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

		virtual int	execute(Client &client) = 0;
		virtual int	execute(Client *client,
			std::map<std::string, Channel *>& channels,
			std::map<int, Client *>& clientLst) = 0;
};

std::ostream   &operator<<(std::ostream &, Command const &);
void			ft_split_word(std::string const& msg,
	std::list<std::string>& args);
std::string		ft_getArg(std::list<std::string>& arg);

# endif

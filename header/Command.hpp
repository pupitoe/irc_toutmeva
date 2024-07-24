/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 19:04:41 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

class Command;

# include <string>
# include <exception>
# include <map>
# include "Channel.hpp"

enum type {
	ERR,
	EMPTY,
	CONNEXION,
	CHANNEL
};

/*
* Polymorphic class
* */
class Command {

	protected :
		std::string	_msg;
		enum type	_type; // Verifier a la fin si c'est toujours utile d'avoir cet attribut

	public :
		Command(std::string msg) throw (Command::UnrecognizedType);
		Command(Command const &);
		virtual ~Command(void);

		std::string	getMsg(void) const;
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

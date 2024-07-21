/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/21 18:22:47 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <exception>

# include <Server.hpp>

enum type {
	ERR,
	EMPTY,
	CONNEXION,
	CHANNEL
};

class Server;
/*
* Polymorphic class
* */
class Command {

	private :
		enum type	_type; // Verifier a la fin si c'est toujours utile d'avoir cet attribut

	protected :
		std::string	_msg;

	public :
		Command(std::string msg) throw (Command::UnrecognizedType);
		Command(Command const &);
		virtual ~Command(void) = 0;

		std::string	getMsg(void) const;
		enum type getType(void) const;

		virtual int	execute(Server *);
		
		class UnrecognizedType : public std::exception {
			const char	*what(void) const throw();
		};
};

std::ostream   &operator<<(std::ostream &, Command const &);

# endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/17 17:07:50 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <exception>

enum type {ERR, EMPTY, CONNEXION, CHANNEL};


/*
* Polymorphic class
* */
class Command{

	private :
		enum type	_type; // Verifier a la fin si c'est toujours utile d'avoir cet attribut

	protected :
		std::string	_msg;

	public :
		Command(std::string msg) throw (Command::UnrecognizedType);
		Command(Command const &);
		~Command(void);

		std::string	getMsg(void) const;
		enum type getType(void) const;
		
		class UnrecognizedType : public std::exception {
			const char	*what(void) const throw();
		};
};

std::ostream   &operator<<(std::ostream &, Command const &);

# endif
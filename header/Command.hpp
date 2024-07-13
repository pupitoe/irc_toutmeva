/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:26:44 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/13 21:13:28 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <exception>


enum type {ERR, CONNEXION, CHANNEL};


/*
* Polymorphic class
* */
class Command{

	private :
		enum type	_type;
		std::string	_msg;

	public :
		Command(char *msg) throw (Command::UnrecognizedType);
		~Command(void);

		enum type getType(void) const;
		
		class UnrecognizedType : public std::exception {
			const char	*what(void) const throw();
		};
};

# endif
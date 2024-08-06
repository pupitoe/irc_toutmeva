/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:14:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/06 16:27:48 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCERROR_HPP
# define IRCERROR_HPP

# include <exception>
# include <string>
# include "Client.hpp"

# define ERR_UNKNOWNERROR		400
# define ERR_INPUTTOOLONG		417
# define ERR_NEEDMOREPARAMS		461
# define ERR_NOSUCHNICK			401

// Don't forget to see if everything is correctly used.
class IRCError : public std::exception {

	private :
		int			_err;
		std::string	_str;
		std::string	_repl;

		IRCError(void);
		
	public :
		IRCError(int err);
		IRCError(int err, std::string str); // code err , infos
		~IRCError(void) throw();
		IRCError(IRCError const &);

		int		getErr(void) const;
		std::string	getError();

		const char  *what(void) const throw();
};

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick);

#endif

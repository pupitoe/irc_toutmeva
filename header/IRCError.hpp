/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:14:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/12 22:19:46 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef IRCERROR_HPP
# define IRCERROR_HPP

# include <exception>
# include <string>
# include "Client.hpp"

# define ERR_UNKNOWNERROR		"400"
# define ERR_NOSUCHNICK			"401"
# define ERR_INPUTTOOLONG		"417"
# define ERR_NONICKNAMEGIVEN	"431"
# define ERR_ERRONEUSNICKNAME	"432"
# define ERR_NICKNAMEINUSE		"433"
# define ERR_NEEDMOREPARAMS		"461"
# define ERR_PASSWDMISMATCH		"464" // also used when no password provided

// Don't forget to see if everything is correctly used.
class IRCError : public std::exception {

	private :
		std::string	_err;
		std::string	_str;
		std::string	_str2;
		std::string	_repl;

		IRCError(void);

		std::string	_gen_reply(void) const;
	public :
		IRCError(std::string err);
		IRCError(std::string err, std::string str); // code err , infos
		~IRCError(void) throw();
		IRCError(IRCError const &);

		std::string	getErr(void) const;
		std::string	getReply(void) const;

		const char  *what(void) const throw();
};

void	ERR_NOSUCHNICK_MSG(Client *client, std::string const& nick);

#endif

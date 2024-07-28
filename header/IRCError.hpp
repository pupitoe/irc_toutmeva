/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:14:12 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/28 11:59:10 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCERROR_HPP
# define IRCERROR_HPP

# include <exception>
# include <string>

# define ERR_UNKNOWNERROR		400
# define ERR_INPUTTOOLONG		417
# define ERR_NEEDMOREPARAMS		461

// Don't forget to see if everything is correctly used.
class IRCError : public std::exception {

	private :
		int			_err;
		std::string	_str;

		IRCError(void);
		
	public :
		IRCError(int err);
		IRCError(int err, std::string str);
		~IRCError(void) throw();
		IRCError(IRCError const &);

		int		getErr(void) const;

		const char  *what(void) const throw();

		/*class NeedMoreParams {
			public :
				const char	*what(void) const throw();
		};

		class Passwdmismatch {
			public :
				const char	*what(void) const throw();
		};

		class AlreadyRegistered {
			public :
				const char	*what(void) const throw();
		};

		class InputTooLong {
			public :
				const char	*what(void) const throw();
		};*/
};

#endif

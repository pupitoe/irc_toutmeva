/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:44:30 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/13 20:15:56 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

# include <string>

class IRCParser {

	private :
	
	public :
		IRCParser(void);
		~IRCParser(void);

		bool	commandIsValid(char *str);
		void	parse(std::string str);
};

# endif
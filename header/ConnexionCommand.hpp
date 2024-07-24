/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionCommand.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:09:33 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/24 21:44:29 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNEXIONCOMMAND_HPP
# define CONNEXIONCOMMAND_HPP

# include <Command.hpp>
# include <IRCError.hpp>
# include <string>

class ConnexionCommand : public Command {

    public :
        ConnexionCommand(std::string msg) throw (IRCError);
        ~ConnexionCommand(void);

        int execute(int socket);
		int	execute(Client *client, std::map<std::string, Channel *>& channels);
};

#endif

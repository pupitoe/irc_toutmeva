/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/07/24 21:46:53 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc_tout_me_va.hpp>
# include <Server.hpp>
#include <ft_error.hpp>
#include <iostream>
#include <vector>
#include <signal.h>

int	g_exiting = 0;

void	handler(int)
{
	std::cout << "Shutdown server !" << std::endl;
	g_exiting = 1;
}

bool	is_number(char *str){
	int	i = 0;

	while (str[i]){
		if (str[i] < 48 || str[i] > 57)
			return (false);
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	int	port;
	// Remplacer les catch par des nombre a l'instantiation des classes errors
	try{
		ConnexionCommand test("PASS 	q\r q q\n	   les amis   		     :test qpwkpqw");
	}
	catch(IRCError &e){
		std::cout << e.what() << std::endl;
	}
	return (0);
	
	if (argc != 3) {
		std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
		return (0);
	}
	port = is_number(argv[1]) ? std::atoi(argv[1]) : -1;
	try
	{
		Server	server(argv[2], port);
		
		signal(SIGINT, &handler);
		if (server.getStatus() == SUCCESS)
		{
			while (!g_exiting)
				server.execut();
		}
		return (0);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

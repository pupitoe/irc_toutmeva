/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/30 20:14:48 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc_tout_me_va.hpp>
#include <ft_error.hpp>
#include <ft_socket.hpp>
#include <iostream>
#include <vector>

#include "signal.h"

int	g_exiting = 0;

void	handler(int)
{
	std::cout << "Shutdown server !" << std::endl;
	g_exiting = 1;
}

int	main(int argc, char **argv)
{
	int	port;
	
	if (argc != 3){
		std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
		return (0);
	}
	port = std::atoi(argv[1]); // Do we have to test the port there ?
	
	std::cout << "THE " << port << std::endl;
	Server	server(argv[2], port);
	
	
/*	Server				server;

	signal(SIGINT, &handler);
	if (server.getStatus() == SUCCESS)
	{
		while (!g_exiting)
		{
			server.searchClient();
			server.clientRecv();
			server.eraseClient();
		}
	}*/
	return (0);
}

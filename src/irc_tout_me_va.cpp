/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 19:46:03 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc_tout_me_va.hpp>
#include <ft_error.hpp>
#include <ft_socket.hpp>
#include <iostream>
#include <vector>

#include <signal.h>

int	g_exiting = 0;

void	handler(int)
{
	std::cout << "Shutdown server !" << std::endl;
	g_exiting = 1;
}

int	main(void)
{
	Server				server;

	signal(SIGINT, &handler);
	if (server.getStatus() == SUCCESS)
	{
		while (!g_exiting)
		{
			server.searchClient();
			server.clientRecv();
			server.eraseClient();
		}
	}
	return (0);
}

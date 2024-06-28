/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 01:12:04 by tlassere         ###   ########.fr       */
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

int	main(void)
{
	int	socket_fd;
	std::vector<int>	client;

	signal(SIGINT, &handler);
	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flue binaire n
	socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	//socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd != -1)
	{
		if (ft_setsoket(socket_fd) == SUCCESS
			&& ft_socket_bind(socket_fd) == SUCCESS)
		{
			while (!g_exiting)
			{
				ft_add_client(socket_fd, client);
				ft_get_message(client);
			}
			ft_free_client(client);
		}
		close(socket_fd);
	}
	return (0);
}

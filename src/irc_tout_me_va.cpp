/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/27 01:02:33 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc_tout_me_va.hpp>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>

#define SIZE_QUEUE 100

int	main(void)
{
	int	socket_fd;
	struct protoent *protocol = getprotobyname("tcp");
	struct sockaddr_in	address;

	int	bclient;

	if (protocol)
	{
		std::cout << protocol->p_name << std::endl;
		std::cout << protocol->p_proto << std::endl;
		//std::cout << protocol->p_aliases[1] << std::endl;
	}
	else
		std::cout << "t nul" << std::endl;
	
	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flue binaire n
	socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	std::cout << socket_fd << std::endl;
	if (socket_fd != -1)
	{
		if (!setsockopt(socket_fd, protocol->p_proto, SO_REUSEADDR | SO_REUSEPORT, NULL, 0))
		{
			std::memset(&address, 0, sizeof(address));
			address.sin_family = AF_INET;
			address.sin_port = 6969;
			address.sin_addr.s_addr = INADDR_LOOPBACK;
			// pour donner une address et un port a notre socket
			if (!bind(socket_fd, (struct sockaddr *)&address, sizeof(address)))
			{
				if (!listen(socket_fd, SIZE_QUEUE))
				{
					bclient = accept(socket_fd, NULL, 0);
					std::cout << bclient << std::endl;
					if (bclient != -1)
						close(bclient);
					bclient = 0;
				}
				else
					std::cout << "fail listen" << std::endl;
			}
			else
				std::cout << "fail to bind" << std::endl;
		}
		else
		{
			std::cout << "setsockopt problem" << std::endl;
			std::cout << "errno -> " << errno << std::endl;
		}
		close(socket_fd);
	}
	return (0);
}

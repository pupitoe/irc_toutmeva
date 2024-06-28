/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/28 03:27:28 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc_tout_me_va.hpp>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>
#include <netinet/tcp.h>

#define SIZE_QUEUE 100

int	main(void)
{
	int	socket_fd;
	struct sockaddr_in	address;

	int	bclient;
	int	bopt;

	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flue binaire n
	socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	std::cout << "socket fd: " << socket_fd << std::endl;
	bopt = 1;
	if (socket_fd != -1)
	{
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &bopt, sizeof(bopt)))
			std::cout << "nike les adress en wait ah ah" << std::endl;
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &bopt, sizeof(bopt)))
			std::cout << "nike les port en wait" << std::endl;
		if (!setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, &bopt, sizeof(bopt)))
		{
			std::memset(&address, 0, sizeof(address));
			address.sin_family = AF_INET;
			address.sin_port = htons(6969); // verifier pourquoi faire ca en detail
			address.sin_addr.s_addr = htonl(INADDR_ANY); // ca aussi ducoup
			// pour donner une address et un port a notre socket
			if (!bind(socket_fd, (struct sockaddr *)&address, sizeof(address)))
			{
				if (!listen(socket_fd, SIZE_QUEUE))
				{
					bclient = accept(socket_fd, NULL, 0);
					std::cout << "client fd: " << bclient << std::endl;

					bool	stopwhile;

					stopwhile = 1;
					while (stopwhile)
					{
						char	buffer[100] = {0};
						recv(bclient, buffer, 100, 0);
						send(bclient, buffer, 100, 0);
						if (!std::strcmp(buffer, "stop\n") || buffer[0] == '\0')
							stopwhile = 0;
						std::cout << buffer << std::endl;
					}
					if (bclient != -1)
						close(bclient);
					bclient = 0;
				}
				else
					std::cout << "fail listen" << std::endl;
			}
			else
			{
				std::cout << "fail to bind" << std::endl;
				std::cout << "errno -> " << errno << std::endl;
			}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/28 18:26:32 by tlassere         ###   ########.fr       */
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
#include <stdio.h>
#include <vector>

#define SIZE_QUEUE 100

static void	ft_error_print(int const err)
{
	if (err == E_SO_REUSEADDR)
		std::cout << "setsockopt: SO_REUSEADDR fail" << std::endl;
	else if (err == E_SO_REUSEPORT)
		std::cout << "setsockopt: SO_REUSEPORT fail" << std::endl;
	else if (err == E_TCP_NODELAY)
		std::cout << "setsockopt: TCP_NODELAY fail" << std::endl;
	else if (err == E_BIND)
		std::cout << "bind: fail" << std::endl;
	else if (err == E_LISTEN)
		std::cout << "listen: fail" << std::endl;
}

static int	ft_setsoket(int const socket_fd)
{
	int	status;
	int	active_opt;

	status = SUCCESS;
	active_opt = 1;
	if (socket_fd != -1)
	{
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
			&active_opt, sizeof(active_opt)))
			status = E_SO_REUSEADDR;
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT,
			&active_opt, sizeof(active_opt)))
			status = E_SO_REUSEPORT;
		if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY,
			&active_opt, sizeof(active_opt)))
			status = E_TCP_NODELAY;
	}
	ft_error_print(status);
	return (status);
}

static int	ft_socket_bind(int const socket_fd)
{
	int					status;
	struct sockaddr_in	address;

	status = SUCCESS;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(6969); // verifier pourquoi faire ca en detail htons
	address.sin_addr.s_addr = htonl(INADDR_ANY); // ca aussi ducoup
	if (socket_fd != -1)
	{
		// pour donner une address et un port a notre socket
		if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)))
			status = E_BIND;
		if (listen(socket_fd, SIZE_QUEUE))
			status = E_LISTEN;
	}
	ft_error_print(status);
	return (status);
}

static void	ft_add_client(int const socket_fd, std::vector<int>& client)
{
	int	new_clien;
	
	new_clien = accept(socket_fd, NULL, 0);
	// a changer car la c'est pour du test
	if (new_clien != -1)
	{
		client.push_back(new_clien);
		std::cout << "client fd: " << new_clien << std::endl;
	}
}

static void	ft_free_client(std::vector<int>& client)
{
	std::vector<int>::iterator it;

	it = client.begin();
	while (it != client.end())
	{
		if (*it != -1)
			close(*it);
		it++;
	}
	client.clear();
}

static void	ft_get_message(std::vector<int> const& client)
{
	bool	stopwhile;
	char	buffer[10000] = {0};
	std::vector<int>::const_iterator it;

	it = client.begin();
	stopwhile = 1;
	while (it != client.end())
	{
		recv(*it, buffer, 10000, MSG_DONTWAIT);
		//std::cout << "recv: "  << recv(*it, buffer, 10000, 0) << std::endl;
		send(*it, buffer, 10000, 0);
		std::cout << buffer;
		it++;
	}
}

int	main(void)
{
	int	socket_fd;
	std::vector<int>	client;

	// pour cree un point de communication (AF_INET c'est le protocol IPV4)
	// SOCK_STREAM permet de creer un flue binaire n
	socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	//socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd != -1)
	{
		if (ft_setsoket(socket_fd) == SUCCESS
			&& ft_socket_bind(socket_fd) == SUCCESS)
		{
			while (client.size() <= 3)
			{
				ft_add_client(socket_fd, client);
				ft_get_message(client);
			}
			ft_free_client(client);
			std::cout << "ah" << std::endl;
		}
		close(socket_fd);
	}
	return (0);
}

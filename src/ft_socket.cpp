/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_socket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 00:53:28 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/30 21:59:54 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ft_socket.hpp>

int	ft_setsoket(int const socket_fd)
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

int	ft_socket_bind(int const socket_fd, int port)
{
	int					status;
	struct sockaddr_in	address;

	status = SUCCESS;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port); // verifier pourquoi faire ca en detail htons
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

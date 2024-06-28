/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_socket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 00:53:28 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 01:10:49 by tlassere         ###   ########.fr       */
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

int	ft_socket_bind(int const socket_fd)
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

void	ft_add_client(int const socket_fd, std::vector<int>& client)
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

void	ft_free_client(std::vector<int>& client)
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

void	ft_get_message(std::vector<int> const& client)
{
	bool	stopwhile;
	int		ret_recv;
	char	buffer[10000] = {0};
	std::vector<int>::const_iterator it;

	it = client.begin();
	stopwhile = 1;
	while (it != client.end())
	{
		recv(6, buffer, 10000, MSG_DONTWAIT);
		ret_recv = recv(*it, buffer, 10000, MSG_DONTWAIT);
		if (ret_recv > 0)
			send(*it, buffer, 10000, 0); // s'il send alors qu'il n'y a pas de connection ca fait tout crash
		std::cout << buffer;
		it++;
	}
}

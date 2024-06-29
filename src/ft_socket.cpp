/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_socket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 00:53:28 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 02:35:05 by tlassere         ###   ########.fr       */
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

static void	ft_get_message_recv(int const client_fd)
{
	int		ret_recv;
	char	buffer[1000] = {0};

	ret_recv = recv(client_fd, buffer, 1000, MSG_DONTWAIT);
	if (ret_recv > 0)
		send(client_fd, buffer, 1000, 0); // s'il send alors qu'il n'y a pas de connection ca fait tout crash
	std::cout << buffer;
}

static int	ft_get_max_fds(std::vector<int> const& client)
{
	std::vector<int>::const_iterator	it;
	int									max_fd;
	
	max_fd = -1;
	it = client.begin();
	while (it != client.end())
	{
		if (max_fd < *it)
			max_fd = *it;
		it++;
	}
	max_fd++;
	return (max_fd);
}

// pour check si le client est fermer il faut just utiliser recv et check son retour de valeur
void	ft_get_message(std::vector<int>& client)
{
	std::vector<int>::const_iterator	it;
	int									max_fd;
	fd_set								rfds;
	struct timeval						tv;
	
	max_fd = ft_get_max_fds(client);
	it = client.begin();
	if (max_fd)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		while (it != client.end())
		{
			FD_SET(*it, &rfds);
			it++;
		}
		it = client.begin();
		if (select(max_fd, &rfds, NULL, NULL, &tv) > 0)
		{
			while (it != client.end())
			{
				if (FD_ISSET(*it, &rfds))
				{
					ft_get_message_recv(*it);
					//std::cout << "FD_ISSET: " << FD_ISSET(*it, &rfds) << std::endl;
				}
				it++;
			}
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_socket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 00:53:39 by tlassere          #+#    #+#             */
/*   Updated: 2024/06/29 02:21:56 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SOCKET_HPP
# define FT_SOCKET_HPP

# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include <sys/select.h>
# include <cstring>
# include <vector>
# include <algorithm>

# include "irc_tout_me_va.hpp"
# include "ft_error.hpp"

# define SIZE_QUEUE 100

void	ft_add_client(int const socket_fd, std::vector<int>& client);
void	ft_free_client(std::vector<int>& client);
void	ft_get_message(std::vector<int>& client);

int		ft_setsoket(int const socket_fd);
int		ft_socket_bind(int const socket_fd);

#endif

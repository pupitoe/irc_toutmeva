/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:13:31 by ggiboury          #+#    #+#             */
/*   Updated: 2024/07/07 18:20:08 by tlassere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCSocket.hpp"

int	IRCSocket::ft_setsocket(int const socket_fd) {
	int	status;
	int	active_opt;

	status = SUCCESS;
	active_opt = 1;
	if (socket_fd != -1)
	{
		//allows you to reuse the address even if it's in a wait state
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
			&active_opt, sizeof(active_opt)))
			status = E_SO_REUSEADDR;
		//allows port to be reused even if in wait state
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT,
			&active_opt, sizeof(active_opt)))
			status = E_SO_REUSEPORT;
		// allows data to be sent even if not complete (e.g. after a d-control)
		if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY,
			&active_opt, sizeof(active_opt)))
			status = E_TCP_NODELAY;
	}
	ft_error_print(status);
	return (status);
}

int	IRCSocket::ft_socket_bind(int const socket_fd, int port) {
	int					status;
	struct sockaddr_in	address;

	status = SUCCESS;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port); // verifier pourquoi faire ca en detail htons
	address.sin_addr.s_addr = htonl(INADDR_ANY); // ca aussi ducoup
	if (socket_fd != -1)
	{
		// to give an address and a port to our socket
		if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)))
			status = E_BIND;
		if (status == SUCCESS && listen(socket_fd, SIZE_QUEUE))
			status = E_LISTEN;
	}
	ft_error_print(status);
	return (status);
}

/*
* List of available port can be found on wikipedia.
* We can't use values under 1024 because rights.
* */ 
bool	IRCSocket::port_is_valid(int p) {
	return (p == 194 || (p >= 6665 && p <= 6669));
}

IRCSocket::IRCSocket(int port)
	throw (IRCSocket::FailedToCreate, IRCSocket::PortInvalid) {
	if (!this->port_is_valid(port))
		throw (IRCSocket::PortInvalid());
	this->_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket_fd == -1 || this->ft_setsocket(this->_socket_fd) == FAIL
		|| this->ft_socket_bind(this->_socket_fd, port) == FAIL)
	{
		if (this->_socket_fd != -1)
			close(this->_socket_fd);
		throw (IRCSocket::FailedToCreate());
	}
}

IRCSocket::~IRCSocket(void) {
	close(this->_socket_fd);
}

int IRCSocket::getSocketFd(void) const {
	return (_socket_fd);
}

const char	*IRCSocket::FailedToCreate::what(void) const throw () {
	return ("Failed to create socket");
}

const char	*IRCSocket::PortInvalid::what(void) const throw () {
	return ("Port is invalid");
}

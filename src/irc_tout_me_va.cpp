/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_tout_me_va.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:59:50 by tlassere          #+#    #+#             */
/*   Updated: 2024/08/17 22:03:24 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc_tout_me_va.hpp>

int	g_exiting = 0;

void	handler(int)
{
	std::cout << "Shutdown server !" << std::endl;
	g_exiting = 1;
}

bool	is_number(char *str)
{
	int	i = 0;

	while (str[i]){
		if (str[i] < 48 || str[i] > 57)
			return (false);
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	int	port;
	
	if (argc != 3) {
		std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
		return (0);
	}
	port = is_number(argv[1]) ? std::atoi(argv[1]) : -1;
	try
	{
		Server	server(argv[2], port);
		
		signal(SIGINT, &handler);
		if (server.getStatus() == SUCCESS)
		{
			while (!g_exiting)
				server.execute();
		}
		return (SUCCESS);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (FAIL);
}

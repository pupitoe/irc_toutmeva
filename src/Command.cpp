/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:52:29 by ggiboury          #+#    #+#             */
/*   Updated: 2024/08/17 18:50:18 by ggiboury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Command.hpp>
#include <iostream>

#include <sstream>
#include <algorithm>

static bool	isCommand(std::string str) {
	unsigned int	i = 0;
	while (str[i]) {
		if (str[i] < 65 || str[i] > 90)
			return (false);
		i++;
	}
	return (true);
}

/**
 * Here's the parsing for our IRC:
 * 	CMD *PARAM
 * 
 * The spaces can be 1 or more spaces (character " ")
 * 
 * CMD := <letters>
 * 
 * PARAM := <optional ":"><letters>
 * 
 * parsing step :
 * 1. Ignore first word if its a command
 * 2. For every word after that
 *	2a. If it begins with ":" , then the rest of msg is final param
 * 	2b. Else, parse normally.
 * */
Command::Command(std::string msg) throw (IRCError) {
	std::cout << "cmd all: " << msg << std::endl << "FIN COMM"<< std::endl;	
	//Command
	ft_split_word(msg, this->_args);
	std::string&	word = this->_args.front();
	for (unsigned int i = 0 ; word[i] != 0 ; i++) {
		word[i] = std::toupper(this->_args.front()[i]);
	}
	if (!isCommand(word))
		throw (IRCError(ERR_UNKNOWNERROR));
	this->_type = EMPTY;
	if (this->_args.size())
		this->_command_name = word;
	this->_nb_arg = this->_args.size();
}

void	ft_split_word(std::string const& msg, std::list<std::string>& args)
{
	std::string			word;
	std::stringstream	str(msg);

	str >> word;
	args.push_back(word);

	//Parameters
	while (str >> word) {
		if (word[0] == ':') {
			if (str.tellg() > 0 && (size_t)str.tellg() < str.str().length())
				word.append(str.str().substr(str.tellg(), str.str().size()));
			word.erase(0, 1);
			args.push_back(word);
			break ;
		}
		args.push_back(word);
	}
}

Command::Command(Command const &ref) : _args(ref.getArgs()),
	_type(ref.getType())
{}

Command::~Command(void)
{}

std::list<std::string>	Command::getArgs(void) const
{
	return (_args);
}

cmd_type	Command::getType(void) const
{
	return (_type);
}

std::ostream	&operator<<(std::ostream &out, Command const &c)
{
	return (out << c.getArgs().front());
}

void	Command::errorMessage(int error, Client *client,
	std::string const& channelName)
{
	if (error == std::atoi(ERR_NEEDMOREPARAMS))
		client->addRPLBuffer(":" + (std::string)SERVERNAME + " 461 "
			+ client->getNickName() + " " + this->_command_name
			+ " :Not enough parameters\n");
	else if (error == ERR_NOSUCHCHANNEL)
		client->addRPLBuffer(":" + (std::string)SERVERNAME + " 403 "
			+ client->getNickName() + " " + channelName
			+ " :No such channel\n");
}

std::string	Command::getArg(void)
{
	return (ft_getArg(this->_args));
}

std::string	ft_getArg(std::list<std::string>& args)
{
	std::string	buffer;
	
	if (args.size())
	{
		buffer = args.front();
		args.pop_front();
	}
	return (buffer);
}

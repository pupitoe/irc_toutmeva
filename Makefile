# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 11:52:59 by tlassere          #+#    #+#              #
#    Updated: 2024/07/22 15:38:05 by ggiboury         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			::= ircserv
SOURCES_PATH 	::= src/
SOURCES_UTIL	::= ft_error IRCSocket Server Client\
					Command ConnexionCommand ChannelCommand IRCError
SOURCES_MAIN	::= irc_tout_me_va $(SOURCES_UTIL)
SOURCES			::= $(foreach buffer, $(SOURCES_MAIN), $(SOURCES_PATH)$(buffer).cpp)
OBJS			::= $(SOURCES:.cpp=.o)
HEADERS			::= header/
HEADERS_CONTENT	::= $(HEADERS)irc_tout_me_va.hpp 
CXXFLAGS		::= -Wall -Wextra -Werror -std=c++98
CXX				::= c++

all: $(NAME)

.cpp.o :
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c -o $@ $<

$(NAME): $(OBJS) $(HEADERS_CONTENT)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

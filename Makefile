# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 11:52:59 by tlassere          #+#    #+#              #
#    Updated: 2024/08/17 18:23:33 by ggiboury         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			::= ircserv
SOURCES_PATH 	::= src/
SOURCES_UTIL	::= ft_error IRCSocket Server Client\
					Command ConnexionCommand ChannelCommand IRCError Channel \
					ChannelMode ChannelCommandJoinPart ChannelCommandMode \
					ChannelErrorMessages Bot BotCommand Morfi
SOURCES_MAIN	::= irc_tout_me_va $(SOURCES_UTIL)
SOURCES			::= $(foreach buffer, $(SOURCES_MAIN), $(SOURCES_PATH)$(buffer).cpp)
OBJS			::= $(SOURCES:.cpp=.o)
HEADERS			::= header/
CXXFLAGS		::= -Wall -Wextra -Werror -std=c++98 -g3
CXX				::= c++

all: $(NAME)

.cpp.o :
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c -o $@ $<

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

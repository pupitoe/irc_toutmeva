# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tlassere <tlassere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 11:52:59 by tlassere          #+#    #+#              #
#    Updated: 2024/05/27 11:59:17 by tlassere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			::= ircserv
SOURCES_PATH 	::= sources/
SOURCES_O		::= $(SOURCES_PATH)$(NAME)
SOURCES			::= $(foreach buffer, $(SOURCES_O), $(buffer).cpp)
OBJS			::= $(SOURCES:.cpp=.o)
HEADERS			::= headers/
HEADERS_CONTENT	::= $(HEADERS)$(NAME).hpp 
CXXFLAGS		::= -Wall -Wextra -Werror -std=c++98

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

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 09:15:50 by jbarratt          #+#    #+#              #
#    Updated: 2024/12/09 14:26:08 by jbarratt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

HEAD = $(NAME:.a=.h)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I.
AR = ar rcs
RM = rm -f

FILES =		ft_dprintf_double \
			ft_dprintf_hex \
			ft_dprintf_int \
			ft_dprintf_str \
			ft_printf

SRCS_DIR = ./
SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(FILES)))

OBJS_DIR = ./
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES)))

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c  $(LIBFT)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	$(RM) $(OBJS) $(OBJS_B)

fclean: clean
	$(RM) $(NAME)

re: clean all

.PHONY: bonus all clean fclean re test

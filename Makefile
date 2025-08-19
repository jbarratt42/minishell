# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 09:15:50 by jbarratt          #+#    #+#              #
#    Updated: 2025/08/19 14:46:00 by jbarratt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SHELL := /bin/bash
HEAD = ./include/$(addsuffix .h, $(NAME))
REPO := ""

CC = cc -O3
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Ilibft
LFLAGS = -Llib -lft -lm -lmlx -lXext -lX11
AR = ar rcs
RM = rm -f
LN = ln -s -f

SRC_DIR := src
OBJ_DIR := obj

SRC_DIRS := $(shell find $(SRC_DIR) -type d)
SRCS := $(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJ_DIRS := $(sort $(dir $(OBJS)))

all: $(NAME)

$(NAME): $(OBJS) lib/libft.a
	$(CC) $(OBJS) $(LFLAGS) -o $(NAME)

$(HEAD): $(SRCS)
	util/update_proto.sh $(HEAD) $(SRCS)

# include/libft.h: libft
# 	mkdir -p include
# 	[[ -f include/libft.h ]] || ln -s ../libft/libft.h include/libft.h
# 
# include/mlx.h: mlx
# 	mkdir -p include
# 	[[ -f include/mlx.h ]] || ln -s ../mlx/mlx.h include/mlx.h
# 
lib/libft.a: libft/*.c libft/libft.h
	mkdir -p lib
	make -C libft
	$(LN) ../libft/libft.a lib/libft.a

lib/libmlx.a: mlx/*.c mlx/mlx.h
	mkdir -p lib
	make -C mlx
	$(LN) ../mlx/libmlx.a lib/libmlx.a

%/%.h:
	make -C $@

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

long: CC += -DLONG
long: re

debug: $(SRCS) $(HEAD) libft mlx
	$(CC) -g $(CFLAGS) $(CPPFLAGS) $(SRCS) $(LFLAGS) -o $(NAME) && \
		gdb $(NAME)
	
clean:
	$(RM) $(OBJS)
	$(RM) -r lib include
	make -C libft clean
	make -C mlx clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean
	make -C mlx clean

re: fclean all

list:
	@echo $(SRCS)

mlx:
	git clone https://github.com/42paris/minilibx-linux.git mlx

libft:
	git clone https://github.com/jbarratt42/libft.git libft

norm:
	norminette $(SRCS) $(HEAD)

repo:
	git clone $(REPO) repo
# 	git clone https://github.com/42paris/minilibx-linux.git repo/mlx
# 	git clone https://github.com/jbarratt42/libft.git repo/libft
	rm -rf repo/mlx/.git repo/libft/.git
	cp $(SRCS) $(HEAD) Makefile repo
	sed -E '/^\$$\(HEAD\): /,/^$$/ s/^/# /' -i repo/Makefile
	cd repo && git add . && git status && git commit && git push

.PHONY: all clean fclean re debug list long norm

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 09:18:55 by chuezeri          #+#    #+#              #
#    Updated: 2025/08/21 12:13:53 by chuezeri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME            := libft.a
TARGET          := test_runner

# Directories
SRC_DIR         := src
OBJ_DIR         := obj
UNITY_DIR       := ${HOME}/unity/src
TEST_DIR        := test
TEST_OBJ_DIR    := test/obj

# Source files
SRCS            := ft_atoi.c \
                   ft_strdup.c \
                   ft_strlcat.c \
                   ft_calloc.c \
				   ft_strlen.c \
                   ft_strncmp.c \
                   ft_striteri.c \
                   ft_memmove.c \
                   ft_strnstr.c \
                   ft_strjoin.c \
                   ft_isalpha.c \
                   ft_isalnum.c \
                   ft_isascii.c \
                   ft_isdigit.c \
                   ft_isprint.c \
                   ft_tolower.c \
                   ft_toupper.c \
			       ft_bzero.c \
			       ft_memset.c \
			       ft_memcmp.c \
			       ft_memchr.c \
			       ft_strchr.c \
			       ft_strrchr.c \
			       ft_strlcpy.c \
			       ft_strcpy.c \
			       ft_memcpy.c \
			       ft_substr.c \
				   ft_strmapi.c \
				   ft_strtrim.c \
				   ft_split.c \
				   ft_itoa.c \
				   ft_putchar_fd.c \
				   ft_putstr_fd.c \
				   ft_putendl_fd.c \
				   ft_putnbr_fd.c


SRCS		:= $(SRCS:%=%)
OBJS		:= $(SRCS:%.c=%.o)

BONUS		:=	ft_lstadd_back.c \
                ft_lstadd_front.c \
				ft_lstclear.c \
				ft_lstdelone.c \
				ft_lstiter.c \
				ft_lstlast.c \
				ft_lstmap.c \
				ft_lstnew.c \
				ft_lstsize.c

BONUS		:= $(BONUS:%=%)
BONUS_OBJS	:= $(BONUS:%.c=%.o)


CRITERION	:= ${HOME}/sgoinfre/linuxbrew/.linuxbrew/Cellar/criterion/2.4.2_1/include

# Include paths
# INCLUDES		:= -I $(INC_DIR) -I $(CRITERION)


# Compiler and flags
CC              := cc
CFLAGS          := -Wall -Wextra -Werror $(INCLUDES)
DEBUG           := -g -fsanitize=leak
# LDFLAGS			:= -L${HOME}/sgoinfre/linuxbrew/.linuxbrew/Cellar/criterion/2.4.2_1/lib -lcriterion
LDFLAGS			:=

# Library archiving flags
LIBFLAGS        := ar rcs

# Clean command
RM              := rm -f

# Default target (build library)
all: $(NAME)

# Run tests
test: $(TARGET)
	./$(TARGET)

# Rule to build the static library
$(NAME): $(OBJS)
	$(LIBFLAGS) $(NAME) $(OBJS)

# Rule to compile the library source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# bonus
bonus:	$(OBJS) $(BONUS_OBJS)
	ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

# Clean rule (remove object files)
clean:
	$(RM) *.o

# Full clean (remove object files and static library)
fclean: clean
	$(RM) $(NAME)

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: clean fclean re

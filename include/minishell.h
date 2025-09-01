/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/01 09:49:05 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#define MINSHELL_DIRECTORY ".minishell"

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>

#include "prlh.h"
#include "lex.h"
#include "parse.h"
#include "../libft/libft.h"

extern int g_status;

typedef struct s_minishell
{
    t_context *context;
    bool interactive;
} t_minishell;

char *ft_strndup(const char *s, size_t n);

/**
 * Copies the first n characters of src to dst.
 **Returns the number of characters copied.
 */
char *ft_strcpy(char *dest, const char *src);

/**
 * Checks for a whitespace character.
 ** Returns non zero if the character is a whitespace, and zero if not.
 */
int ft_isspace(int c);
#endif

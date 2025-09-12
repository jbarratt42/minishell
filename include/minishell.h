/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/12 18:42:32 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#define MINISHELL_PROMPT "minishell$ "
#define MINSHELL_DIRECTORY ".minishell"

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>

#include "prlh.h"
#include "lex.h"
#include "signals.h"
#include "parse.h"
#include "exec.h"
#include "util.h"
#include "../libft/libft.h"
#ifdef DEBUG
#include "print_tree.h"
#endif
#ifdef DEBUG
#include "print_tree.h"
#endif

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

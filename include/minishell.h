/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/29 16:17:43 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MINISHELL_PROMPT "minishell$ "
# define MINSHELL_DIRECTORY ".minishell"

/**
 * !! DO NOT CHANGE THE IMPORT ORDER
 */
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>

# include "../libft/libft.h"
# include "prlh.h"
# include "exec.h"
# include "builtin.h"
# include "lex.h"
# include "parse.h"
# include "signals.h"
# include "util.h"

# define PATH_MAX 1024

extern int		g_status;

typedef struct s_minishell
{
	t_context	*context;
	bool		interactive;
}				t_minishell;

char			*ft_strndup(const char *s, size_t n);

/**
 * Copies the first n characters of src to dst.
 **Returns the number of characters copied.
 */
char			*ft_strcpy(char *dest, const char *src);

/**
 * Checks for a whitespace character.
 ** Returns non zero if the character is a whitespace, and zero if not.
 */
int				ft_isspace(int c);
#endif

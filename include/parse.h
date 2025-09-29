/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/29 13:33:07 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "../libft/libft.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_node	t_node;

typedef struct s_operator
{
	enum e_token_type	type;
	t_node				*left;
	t_node				*right;
}						t_operator;

union					u_data
{
	t_operator			op;
	t_token				*tokens;
};

typedef struct s_node
{
	bool				is_terminal;
	union u_data		data;
}						t_node;

t_node					*parse(t_token **token, int min_precedence);
int						is_arg(t_token *token);
int						try_open(char *path, int flags);
void					parse_redirect(t_token *token, int fd[2]);
t_node					*parse_command(t_token *token, char *path, char **argv,
							int *fd);
void					free_node(t_node *node);
#endif

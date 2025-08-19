/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/14 15:12:01 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <bool.h>

enum e_toktype
{
	WORD,
	FILE_IN,
	FILE_OUT,
	FILE_APPEND,
	HEREDOC,
	OPEN,
	CLOSE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	MAX_TOKTYPE
};

typedef struct s_token
{
	char			*txt;
	e_tokype		type;
	struct s_token	*next;
}	t_token;

union u_data
{
	t_operator	op;
	t_token		*tokens;
};

typedef struct s_node	t_node;

typedef struct s_operator
{
	e_toktype	type;
	t_node		*left;
	t_node		*right;
}	t_operator;

typedef struct s_node
{
	bool	is_terminal;
	u_data	data;
}	t_node;

/* env is a modifiable duplicate of main's env */
typedef struct s_context
{
	int		argc;
	char	**argv;
	char	**env;
	int		status;
	char	*line;
	t_token	*tokens;
	t_node	*tree;
}

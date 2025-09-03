/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/03 12:56:14 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
#define PARSE_H
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include "../libft/libft.h"

/*
typedef enum e_toktype
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
} t_toktype;

typedef struct s_token
{
	char *txt;
	enum e_toktype type;
	struct s_token *next;
} t_token;

*/
typedef struct s_node t_node;

typedef struct s_operator
{
	enum e_token_type type;
	t_node *left;
	t_node *right;
} t_operator;

union u_data
{
	t_operator op;
	t_token *tokens;
};

typedef struct s_node
{
	bool is_terminal;
	union u_data data;
} t_node;

/* env is a modifiable duplicate of main's env */
/*
typedef struct s_context
{
	int argc;
	char **argv;
	char **env;
	int status;
	char *input;
	t_token *tokens;
	t_node *tree;
	int open_fd[3];
} t_context;
*/

/* prototypes */
/* src/parse/parse.c */
t_node *parse(t_token **token, int min_precedence);
int is_arg(t_token *token);
int try_open(char *path, int flags);
void parse_redirect(t_token *token, int fd[2]);
t_node *parse_command(t_token *token, char *path, char **argv, int *fd);
int exec(t_token *tokens, int fd[3], char **env);
// /* src/util/env.c */
// char **copy_env(char **env);
// char **push_env(char **env, char *str);
// /* src/util/free.c */
// void free_token(t_token *token);
// // void free_node(t_node *node);
// void free_context(t_context *context);
// /* src/util/init.c */
// bool init_context(int argc, char **argv, char **env, t_context *context);
// /* src/util/print_tree.c */
// void print_indentation(int depth);
// void print_tree_structure(t_node *node, int depth);
// /* src/exec/exec.c */
// int collect_process(pid_t pid);
// pid_t exec_terminal(t_token *tokens, t_context *context);
#endif

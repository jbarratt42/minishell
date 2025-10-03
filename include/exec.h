/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:30:37 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/03 10:44:30 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

/* src/exec/assign.c */
void	delete_tokens(t_token **token, size_t len);
bool	is_command(t_token *token);
bool	assign(t_token **token, t_context *context);
bool	heredoc(t_token *token, t_context *context);
bool	reassign_fd(t_token *token, t_context *context);
bool	redirect(t_token **token, t_context *context);
t_token	**separate_words(t_token *token);
void	dequote(char *str);
bool	expand_tokens(t_token **token, t_context *context);
/* src/exec/exec.c */
char	*append_path(char *a, char *b);
char	*search_path(char *s, char **env);
bool	try_dup2(int open[3]);
char	**get_args(t_token *tokens);
char	*get_path(t_token *tokens, char **env);
bool	is_builtin(t_token *token);
bool	exec_builtin(t_token *tokens, t_context *context);
bool	exec_preprocess(t_token **tokens, t_context *context);
bool	set_exp_vars(t_token **tokens, t_context *context);
pid_t	exec_terminal(t_token **tokens, t_context *context);
bool	exec_sequential(t_node *node, t_context *context);
pid_t	traverse(t_node *node, t_context *context);
bool	try_close2(int open[2]);
bool	try_pipe(int fds[2]);
#endif

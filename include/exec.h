/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:30:37 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/06 11:21:12 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

/* prototypes */
/* src/exec//assign.c */
void	delete_tokens(t_token **token, size_t len);
bool	is_command(t_token *token);
bool	assign(t_token **token, t_context *context);
bool	redirect(t_token **token, t_context *context);
/* src/exec//collect.c */
int		collect(int pid);
int		collect2(int pids[2]);
/* src/exec//exec.c */
bool	exec_builtin(t_token *tokens, t_context *context);
bool	exec_preprocess(t_token **tokens, t_context *context);
pid_t	exec_terminal(t_token **tokens, t_context *context);
bool	exec_sequential(t_node *node, t_context *context);
/* src/exec//handle.c */
pid_t	handle_builtins(t_token **tokens, t_context *context);
void	check_path_child(char *path, t_token **tokens);
void	handle_execve_fail(char *path);
/* src/exec//help.c */
char	**get_args(t_token *tokens);
bool	is_builtin(t_token *token);
bool	set_exp_vars(t_token **tokens, t_context *context);
bool	cleanup_parent(t_context *context);
/* src/exec//helpers.c */
bool	expand_tokens(t_token **token, t_context *context);
bool	reassign_fd(t_token *token, t_context *context);
/* src/exec//heredoc.c */
bool	heredoc(t_token *token, t_context *context);
/* src/exec//path.c */
char	*append_path(char *a, char *b);
char	*search_path(char *s, char **env);
char	*get_path(t_token *tokens, char **env);
/* src/exec//traverse.c */
void	set_status(pid_t pids[2], t_context *context);
pid_t	traverse(t_node *node, t_context *context);
/* src/exec//try.c */
bool	try_close2(int open[2]);
bool	try_pipe(int fds[2]);
bool	try_dup2(int open[3]);
/* src/exec//utils.c */
t_token	**separate_words(t_token *token);
void	dequote(char *str);
#endif

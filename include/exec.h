/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:30:37 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/12 12:23:48 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
#define EXEC_H
#include "minishell.h"
/* src/exec/assign.c */
void delete_tokens(t_token **token, size_t len);
_Bool is_command(t_token *token);
_Bool assign(t_token **token, t_context *context);
_Bool heredoc(t_token *token, t_context *context);
_Bool reassign_fd(t_token *token, t_context *context);
_Bool redirect(t_token **token, t_context *context);
t_token **separate_words(t_token *token);
_Bool expand_tokens(t_token **token, t_context *context);
/* src/exec/exec.c */
char *append_path(char *a, char *b);
char *search_path(char *s, char **env);
_Bool try_dup2(int open[3]);
char **get_args(t_token *tokens);
char *get_path(t_token *tokens, char **env);
_Bool is_builtin(t_token *token);
_Bool exec_builtin(t_token *tokens, t_context *context);
_Bool exec_preprocess(t_token **tokens, t_context *context);
_Bool set_exp_vars(t_token **tokens, t_context *context);
pid_t exec_terminal(t_token **tokens, t_context *context);
bool exec_sequential(t_node *node, t_context *context);
pid_t traverse(t_node *node, t_context *context);
#endif

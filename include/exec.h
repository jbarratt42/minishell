/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:30:37 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/06 13:31:12 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
#define EXEC_H
#include "minishell.h"
/* src/exec/exec.c */
bool try_dup2(int open[3]);
char **get_args(t_token *tokens);
char *get_path(t_token *tokens, char **env);
bool is_builtin(t_token *token);
void exec_builtin(t_token *token, t_context *context);
pid_t exec_terminal(t_token *tokens, t_context *context);
void exec_sequential(t_node *node, t_context *context);
pid_t traverse(t_node *node, t_context *context);

#endif
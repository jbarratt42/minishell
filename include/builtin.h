/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 21:24:15 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/12 21:24:16 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
#define BUILTIN_H

// Builtin command functions
int builtin_echo(t_token *tokens, t_context *context);
int builtin_cd(t_token *tokens, t_context *context);
int builtin_pwd(t_token *tokens, t_context *context);
int builtin_export(t_token *tokens, t_context *context);
int builtin_unset(t_token *tokens, t_context *context);
int builtin_env(t_token *tokens, t_context *context);
int builtin_exit(t_token *tokens, t_context *context);

// Helper functions
bool is_builtin_command(const char *cmd);
int execute_builtin(const char *cmd, t_token *tokens, t_context *context);

#endif

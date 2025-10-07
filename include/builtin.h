/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 21:24:15 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 13:56:44 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include <stdbool.h>
// Builtin command functions
int		builtin_echo(t_token *tokens, t_context *context);
int		builtin_cd(t_token *tokens, t_context *context);
int		builtin_pwd(t_token *tokens, t_context *context);
int		builtin_export(t_token *tokens, t_context *context);
int		builtin_unset(t_token *tokens, t_context *context);
int		builtin_env(t_token *tokens, t_context *context);
int		builtin_exit(t_token *tokens, t_context *context);
int		builtin_history(t_token *tokens, t_context *context);

// Helper functions
char	*get_home_path(t_context *context);

void	sort_env_vars(char **env, int count);
void	print_exported_vars(char **env);
bool	is_valid_identifier(const char *str);
bool	is_builtin_command(const char *cmd);
int		execute_builtin(const char *cmd, t_token *tokens, t_context *context);

void	update_pwd_vars(t_context *context, char *old_pwd);
// cd helpers
t_token	*advance_to_arg(t_token *tokens);
int		change_dir_and_update(t_context *context, const char *path,
			char *old_pwd);
int		ensure_single_arg(t_token *arg);
int		resolve_path_value(t_token *arg, t_context *context, char **out_path,
			int *print_pwd);
void	print_current_pwd_if_requested(int print_pwd);

#endif

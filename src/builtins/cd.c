/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:45:55 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 13:56:34 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_path(t_context *context)
{
	char	*home;

	home = ft_getenv("HOME", context->env);
	if (!home)
		home = ft_getenv("HOME", context->local);
	return (home);
}

static void	set_pwd_env(t_context *context, const char *new_pwd)
{
	char	*pwd_var;

	pwd_var = ft_strjoin("PWD=", new_pwd);
	if (pwd_var)
		context->env = set_env(pwd_var, context->env);
}

static void	set_oldpwd_env_and_free_old(char *old_pwd, t_context *context)
{
	char	*old_pwd_var;

	if (!old_pwd)
		return ;
	old_pwd_var = ft_strjoin("OLDPWD=", old_pwd);
	if (old_pwd_var)
		context->env = set_env(old_pwd_var, context->env);
	free(old_pwd);
}

void	update_pwd_vars(t_context *context, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: getcwd");
		return ;
	}
	set_pwd_env(context, new_pwd);
	set_oldpwd_env_and_free_old(old_pwd, context);
	free(new_pwd);
}

int	builtin_cd(t_token *tokens, t_context *context)
{
	t_token	*arg;
	char	*old_pwd;
	char	*path;
	int		print_pwd;
	int		res;

	arg = advance_to_arg(tokens);
	if (ensure_single_arg(arg))
		return (1);
	old_pwd = getcwd(NULL, 0);
	res = resolve_path_value(arg, context, &path, &print_pwd);
	if (res)
	{
		free(old_pwd);
		return (res);
	}
	if (change_dir_and_update(context, path, old_pwd))
		return (1);
	print_current_pwd_if_requested(print_pwd);
	return (0);
}

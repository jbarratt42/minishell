/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:45:55 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 12:46:33 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

static char	*get_home_path(t_context *context)
{
	char	*home;

	home = ft_getenv("HOME", context->env);
	if (!home)
		home = ft_getenv("HOME", context->local);
	return (home);
}

static void	update_pwd_vars(t_context *context, char *old_pwd)
{
	char	*new_pwd;
	char	*pwd_var;
	char	*old_pwd_var;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: getcwd");
		return ;
	}
	pwd_var = ft_strjoin("PWD=", new_pwd);
	if (pwd_var)
		context->env = set_env(pwd_var, context->env);
	if (old_pwd)
	{
		old_pwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (old_pwd_var)
		{
			context->env = set_env(old_pwd_var, context->env);
		}
		free(old_pwd);
	}
	free(new_pwd);
}

int	builtin_cd(t_token *tokens, t_context *context)
{
	char	*path;
	char	*old_pwd;
	t_token	*current;
	char	*new_pwd;

	current = tokens;
	if (current && current->type == WORD)
		current = current->next;
	if (current && current->type == WORD && current->next
		&& current->next->type == WORD)
		return (err_printf("cd: too many arguments\n"), 1);
	old_pwd = getcwd(NULL, 0);
	if (!current || current->type != WORD)
	{
		path = get_home_path(context);
		if (!path)
		{
			err_printf("cd: HOME not set\n");
			free(old_pwd);
			return (1);
		}
	}
	else
		path = current->value;
	if (ft_strcmp(path, "-") == 0)
	{
		path = ft_getenv("OLDPWD", context->env);
		if (!path)
		{
			err_printf("cd: OLDPWD not set\n");
			free(old_pwd);
			return (1);
		}
	}
	else if (path[0] == '-')
	{
		err_printf("cd: %s: invalid option\n", path);
		err_printf("cd: usage: cd [-L|[-P [-e]]] [-@] [dir]\n");
		free(old_pwd);
		return (2);
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	update_pwd_vars(context, old_pwd);
	if (current && current->type == WORD && ft_strcmp(current->value, "-") == 0)
	{
		new_pwd = getcwd(NULL, 0);
		if (new_pwd)
		{
		ft_printf("%s\n", new_pwd);
			free(new_pwd);
		}
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 13:49:24 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 13:52:20 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*advance_to_arg(t_token *tokens)
{
	if (tokens && tokens->type == WORD)
		return (tokens->next);
	return (tokens);
}

int	ensure_single_arg(t_token *arg)
{
	if (arg && arg->type == WORD && arg->next && arg->next->type == WORD)
		return (err_printf("cd: too many arguments\n"), 1);
	return (0);
}

int	resolve_path_value(t_token *arg, t_context *context, char **out_path,
		int *print_pwd)
{
	char	*path;

	*print_pwd = 0;
	if (!arg || arg->type != WORD)
	{
		path = get_home_path(context);
		if (!path)
			return (err_printf("cd: HOME not set\n"), 1);
	}
	else
		path = arg->value;
	if (ft_strcmp(path, "-") == 0)
	{
		path = ft_getenv("OLDPWD", context->env);
		if (!path)
			return (err_printf("cd: OLDPWD not set\n"), 1);
		*print_pwd = 1;
	}
	else if (path[0] == '-')
		return (err_printf("cd: %s: invalid option\n", path),
			err_printf("cd: usage: cd [-L|[-P [-e]]] [-@] [dir]\n"), 2);
	*out_path = path;
	return (0);
}

int	change_dir_and_update(t_context *context, const char *path, char *old_pwd)
{
	if (chdir(path) == -1)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	update_pwd_vars(context, old_pwd);
	return (0);
}

void	print_current_pwd_if_requested(int print_pwd)
{
	char	*new_pwd;

	if (!print_pwd)
		return ;
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		ft_printf("%s\n", new_pwd);
		free(new_pwd);
	}
}

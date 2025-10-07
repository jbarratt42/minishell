/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:44:47 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 12:46:01 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin_command(const char *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", "history", NULL};
	int			i;

	if (!cmd)
		return (false);
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

int	execute_builtin(const char *cmd, t_token *tokens, t_context *context)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (builtin_echo(tokens, context));
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (builtin_cd(tokens, context));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (builtin_pwd(tokens, context));
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (builtin_export(tokens, context));
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (builtin_unset(tokens, context));
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (builtin_env(tokens, context));
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (builtin_exit(tokens, context));
	else if (ft_strncmp(cmd, "history", 7) == 0)
		return (builtin_history(tokens, context));
	return (1);
}

int	builtin_history(t_token *tokens, t_context *context)
{
	char	*history_path;
	int		ret;

	(void)tokens;
	(void)context;
	history_path = get_history_path();
	if (!history_path)
		return (EXIT_FAILURE);
	ret = print_history(history_path);
	free(history_path);
	return (ret);
}

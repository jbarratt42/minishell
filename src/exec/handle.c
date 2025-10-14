/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:46:49 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/13 11:36:50 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t handle_builtins(t_token **tokens, t_context *context)
{
	pid_t pid;

	if (!context->is_pipeline)
	{
		try_dup2(context->open);
		exec_builtin(*tokens, context);
		context->open[0] = 0;
		context->open[1] = 1;
		context->open[2] = -1;
		return (0);
	}
	pid = fork();
	if (pid == 0)
	{
		try_dup2(context->open);
		exec_builtin(*tokens, context);
		exit(context->status);
	}
	if (pid > 0)
	{
		if (!cleanup_parent(context))
			return (-1);
		return (pid);
	}
	return (-1);
}

void check_path_child(char *path, t_token **tokens)
{
	struct stat st;

	if (!path)
	{
		err_printf("%s: command not found\n", (*tokens)->value);
		exit(127);
	}
	if (access(path, F_OK) == -1)
	{
		err_printf("%s: No such file or directory\n", path);
		exit(127);
	}
	if (access(path, X_OK) == -1)
	{
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			err_printf("%s: Is a directory\n", path);
			exit(126);
		}
		else
		{
			err_printf("%s: Permission denied\n", path);
			exit(126);
		}
	}
}

void handle_execve_fail(char *path)
{
	struct stat st;

	if (errno == ENOENT)
	{
		err_printf("%s: No such file or directory\n", path);
		exit(127);
	}
	else if (errno == EACCES)
	{
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			err_printf("%s: Is a directory\n", path);
			exit(126);
		}
		else
		{
			err_printf("%s: Permission denied\n", path);
			exit(126);
		}
	}
	perror("exec_terminal");
	exit(1);
}

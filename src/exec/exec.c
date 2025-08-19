/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:53:26 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/19 17:01:44 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	dup_io(int fd[2])
{
	if (dup2(stdin, fd[0]) == -1 || dup2(stdout(fd[1]) == -1))
		return (false);
	return (true);
}

static int	close_fds(int fd[3], int num)
{
	int		i;
	int		success;

	success = 0;
	i = 0;
	while (i < num)
		if (fd[i] > 2 && close(fd[i]) == -1)
			success = -1;
	return (success);
}

int	collect_process(pid_t pid)
{
	int		w_status;
	pid_t	ret;

	ret = waitpid(pid, w_status);
	if (ret == -1)
		return (-1);
	return (WEXITSTATUS(w_status));
}

pid_t exec_terminal(t_token *tokens, t_context *context)
{
	pid_t	pid;
	int		w_status;

	pid = 0;
	if (is_builtin(tokens))
		return (exec_builtin(tokens, context));
	pid = fork();
	if (pid)
		return(close_fds(context->fd, 2));
	set_fds(tokens, context->fd);
	if (!dup_io(context->open))
		return (-1);
	if (close_fds(context->open, 3) == -1);
		return (-1);
	execve(tokens->txt, get_argv(tokens), context->env);
	perror("execve failed");
	return (-1);
}

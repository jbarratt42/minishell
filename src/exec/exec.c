/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:53:26 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/23 21:59:18 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool dup_io(int fd[2])
{
	if (dup2(STDIN_FILENO, fd[0]) == -1 || dup2(STDOUT_FILENO, (fd[1]) == -1))
		return (false);
	return (true);
}

static int close_fds(int fd[3], int num)
{
	int i;
	int success;

	success = 0;
	i = 0;
	while (i < num)
		if (fd[i] > 2 && close(fd[i]) == -1)
			success = -1;
	return (success);
}

int collect_process(pid_t pid)
{
	int w_status;
	pid_t ret;

	ret = waitpid(pid, &w_status, 0);
	if (ret == -1)
		return (-1);
	return (WEXITSTATUS(w_status));
}
void set_fds(t_token *tokens, int fds[3])
{
	(void)tokens;
	(void)fds;
}

char *get_argv(t_token *tokens)
{
	(void)tokens;
	return NULL;
}

pid_t exec_terminal(t_token *tokens, t_context *context)
{
	pid_t pid;
	// int w_status;

	pid = 0;
	if (is_builtin(tokens))
		return (exec_builtin(tokens, context));
	pid = fork();
	if (pid)
		return (close_fds(context->fds, 2));
	set_fds(tokens, context->fds);
	if (!dup_io(&context->open))
		return (-1);
	if (close_fds(&context->open, 3) == -1)
		;
	return (-1);
	execve(tokens->txt, (char *const *)get_argv(tokens), context->env);
	perror("execve failed");
	return (-1);
}

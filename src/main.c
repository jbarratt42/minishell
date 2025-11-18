/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:09:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/11/18 15:08:05 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	g_status = 0;

static void	wait_and_set_status(pid_t pid, t_context *context)
{
	if (waitpid(pid, &context->status, 0) == -1)
		perror("main");
	signal(SIGINT, signal_handler);
	context->status = WEXITSTATUS(context->status);
}

void	parse_and_execute(t_context *context, bool is_interactive)
{
	pid_t	pid;
	t_token	*tmp;

	if (is_interactive)
		add_history(context->input);
	context->tokens = lex(context->input);
	if (!context->tokens)
		cleanup_and_exit(context);
	tmp = context->tokens;
	context->tree = parse(&tmp, 0);
	if (!context->tree)
	{
		err_printf("syntax error\n");
		g_status = 2;
		return ;
	}
	pid = traverse(context->tree, context);
	if (pid == -1)
		context->status = 1;
	if (pid && pid != -1)
		wait_and_set_status(pid, context);
	if (!is_interactive)
		cleanup_and_exit(context);
}

static void	free_for_input(t_context *context)
{
	free(context->input);
	context->input = NULL;
	free_tokens(context->tokens);
	context->tokens = NULL;
	free_node(context->tree);
	context->tree = NULL;
	context->is_pipeline = false;
	context->is_heredoc = false;
	context->open[0] = 0;
	context->open[1] = 1;
	context->open[2] = -1;
}

static bool	process_input(t_context *context, bool is_interactive)
{
	char	*tmp;

	if (g_status == 130 && context->is_execve)
		write(STDOUT_FILENO, "!", 1);
	context->is_execve = false;
	tmp = readline(MINISHELL_PROMPT);
	if (g_status)
	{
		context->status = g_status;
		g_status = 0;
	}
	context->input = ft_strtrim(tmp, " \t\n");
	free(tmp);
	if (!context->input)
	{
		if (!is_interactive)
			return (false);
		builtin_exit(NULL, context);
	}
	if (context->input && !ft_strlen(context->input))
		return (free(context->input), true);
	parse_and_execute(context, is_interactive);
	free_for_input(context);
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	static t_context	context = {0};
	bool				is_interactive;

	is_interactive = isatty(STDIN_FILENO);
	init_context(&context, argc, argv, env);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	while (process_input(&context, is_interactive))
		;
	clear_history();
	free_context(&context);
	return (EXIT_SUCCESS);
}

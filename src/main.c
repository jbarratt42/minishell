/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:09:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/11/08 11:41:23 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_status = 0;

static void	wait_and_set_status(pid_t pid, t_context *context)
{
	if (waitpid(pid, &context->status, 0) == -1)
		perror("main");
	context->status = WEXITSTATUS(context->status);
}

void	parse_and_execute(t_context *context, bool is_interactive)
{
	pid_t	pid;

	if (is_interactive)
		add_history(context->input);
	context->tokens = lex(context->input);
	if (!context->tokens)
	{
		free_node(context->tree);
		exit(context->status);
	}
	context->tree = parse(&context->tokens, 0);
	if (!context->tree)
	{
		err_printf("syntax error\n");
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

	tmp = readline(MINISHELL_PROMPT);
	context->input = ft_strtrim(tmp, " \t\n");
	free(tmp);
	if (!context->input)
	{
		if (!is_interactive)
			return (false);
		builtin_exit(NULL, context);
	}
	if (context->input && !ft_strlen(context->input))
	{
		free(context->input);
		return (true);
	}
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
	while (process_input(&context, is_interactive))
		;
	clear_history();
	free_context(&context);
	return (EXIT_SUCCESS);
}

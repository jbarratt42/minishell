/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:09:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 17:10:47 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_status = 0;

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
	pid = traverse(context->tree, context);
	if (pid == -1)
		context->status = 1;
	if (pid && pid != -1)
	{
		if (waitpid(pid, &context->status, 0) == -1
			|| !WIFEXITED(context->status))
			perror("main");
		context->status = WEXITSTATUS(context->status);
	}
	if (!is_interactive)
		cleanup_and_exit(context);
}

int	main(int argc, char **argv, char **env)
{
	static t_context	context = {0};
	bool				is_interactive;

	is_interactive = isatty(STDIN_FILENO);
	init_context(&context, argc, argv, env);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		context.input = readline(MINISHELL_PROMPT);
		if (!context.input)
		{
			if (!is_interactive)
				break ;
			builtin_exit(NULL, &context);
		}
		parse_and_execute(&context, is_interactive);
		free_tokens(context.tokens);
		free_node(context.tree);
	}
	clear_history();
	free_tokens(context.tokens);
	free_node(context.tree);
	return (EXIT_SUCCESS);
}

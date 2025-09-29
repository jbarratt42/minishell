/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:09:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 13:53:48 by chuezeri         ###   ########.fr       */
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
		cleanup_and_exit(context);
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
	t_context	context;
	bool		is_interactive;

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
			builtin_exit(context.tokens, &context);
		}
		parse_and_execute(&context, is_interactive);
		free_tokens(context.tokens);
		free_node(context.tree);
		context.tokens = NULL;
		context.tree = NULL;
	}
	free_tokens(context.tokens);
	free_node(context.tree);
	free_context(&context);
	return (EXIT_SUCCESS);
}

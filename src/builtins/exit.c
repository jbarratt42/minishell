/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:44:34 by chuezeri          #+#    #+#             */
/*   Updated: 2025/11/19 12:22:07 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_too_many_args(t_token *arg)
{
	if (!arg)
		return (false);
	if (!is_numeric(arg->value))
		return (false);
	return (arg->next && arg->next->type == WORD);
}

static int	normalize_exit_code(long code)
{
	if (code < 0)
		return (256 + (code % 256));
	if (code > 255)
		return (code % 256);
	return ((int)code);
}

static int	parse_exit_code(const char *value, int *out_code)
{
	long	parsed;

	if (!is_numeric(value))
		return (-1);
	parsed = ft_atoi(value);
	*out_code = normalize_exit_code(parsed);
	return (0);
}

static void	exit_shell(t_context *context, int exit_code)
{
	ft_printf("exit\n");
	/*
	if (ft_getenv("MINISHLVL", context->env)
			&& ft_strncmp(ft_getenv("MINISHLVL", context->env), "1", 10) == 0)
		write(1, "\n", 1);
		*/
	clear_history();
	free_context(context);
	exit(exit_code);
}

int	builtin_exit(t_token *tokens, t_context *context)
{
	t_token	*current;
	int		exit_code;

	current = tokens;
	if (current && current->type == WORD)
		current = current->next;
	exit_code = context->status;
	if (current && current->type == WORD)
	{
		if (has_too_many_args(current))
		{
			err_printf("exit: too many arguments\n");
			return (1);
		}
		if (parse_exit_code(current->value, &exit_code) < 0)
		{
			err_printf("exit: %s: numeric argument required\n", current->value);
			exit_shell(context, 2);
		}
	}
	exit_shell(context, exit_code);
	return (exit_code);
}

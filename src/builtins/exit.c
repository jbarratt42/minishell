/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:44:34 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 13:50:07 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_numeric(const char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	builtin_exit(t_token *tokens, t_context *context)
{
	t_token	*current;
	int		exit_code;

	current = tokens;
	if (current && current->type == WORD)
		current = current->next;
	exit_code = 0;
	if (current && current->type == WORD)
	{
		if (current->next && current->next->type == WORD
			&& is_numeric(current->value))
		{
			fprintf(stderr, "exit: too many arguments\n");
			return (1);
		}
		if (!is_numeric(current->value))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n",
				current->value);
			exit_code = 2;
		}
		else
		{
			exit_code = ft_atoi(current->value);
			if (exit_code < 0)
				exit_code = 256 + (exit_code % 256);
			else if (exit_code > 255)
				exit_code = exit_code % 256;
		}
	}
	printf("exit\n");
	cleanup_and_exit(context);
	return (exit_code);
}

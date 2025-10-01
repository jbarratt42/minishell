/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:46:42 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 12:48:03 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(t_token *tokens, t_context *context)
{
	bool	newline;
	t_token	*current;
	int		i;

	(void)context;
	newline = true;
	current = tokens;
	if (current && current->type == WORD)
		current = current->next;
	while (current && current->type == WORD && current->value[0] == '-')
	{
		i = 1;
		while (current->value[i] == 'n')
			i++;
		if (!(current->value[i] == '\0' && i > 1))
			break ;
		newline = false;
		current = current->next;
	}
	while (current && current->type == WORD)
	{
		ft_printf("%s", current->value);
		current = current->next;
		if (current && current->type == WORD)
			ft_printf(" ");
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

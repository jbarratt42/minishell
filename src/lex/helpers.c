/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 13:21:56 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 13:26:40 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '-');
}

void	lexer_error(const char *msg, int pos, char *token)
{
	(void)msg;
	(void)pos;
	(void)token;
	err_printf("minishell: %s %s\n", msg, "");
}

void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] && ft_isspace((unsigned char)input[*i]))
		(*i)++;
}

int	validate_sequence(t_token *prev, t_token *next, int i, t_token *head)
{
	if (!next)
		return (1);
	if ((prev->type == AND || prev->type == OR) && (next->type == AND
			|| next->type == OR || next->type == PIPE))
	{
		lexer_error("syntax error near unexpected token", i, prev->value);
		free_tokens(head->next);
		return (0);
	}
	if (prev->type == PIPE && next->type == PIPE)
	{
		lexer_error("syntax error near unexpected token", i, prev->value);
		free_tokens(head->next);
		return (0);
	}
	return (1);
}

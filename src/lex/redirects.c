/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 13:22:57 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 13:25:37 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lex_pipe_or(const char *input, int *i)
{
	t_token	*t;

	if (input[*i] != '|')
		return (NULL);
	if (input[*i + 1] == '|')
	{
		t = token_new(OR, "||", *i);
		*i += 2;
		return (t);
	}
	return (token_new(PIPE, "|", (*i)++));
}

t_token	*lex_and(const char *input, int *i)
{
	t_token	*t;

	if (!(input[*i] == '&' && input[*i + 1] == '&'))
		return (NULL);
	t = token_new(AND, "&&", *i);
	*i += 2;
	return (t);
}

t_token	*lex_redir_in(const char *input, int *i)
{
	t_token	*t;

	if (input[*i] != '<')
		return (NULL);
	if (input[*i + 1] == '<')
	{
		t = token_new(HEREDOC, "<<", *i);
		*i += 2;
		return (t);
	}
	return (token_new(REDIR_IN, "<", (*i)++));
}

t_token	*lex_redir_out(const char *input, int *i)
{
	t_token	*t;

	if (input[*i] != '>')
		return (NULL);
	if (input[*i + 1] == '>')
	{
		t = token_new(REDIR_APPEND, ">>", *i);
		*i += 2;
		return (t);
	}
	return (token_new(REDIR_OUT, ">", (*i)++));
}

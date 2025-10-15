/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:16:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/15 15:31:52 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*lex_word(const char *input, int *i)
{
	int		start;
	char	delim;
	t_token	*token;
	char	*str;

	start = *i;
	delim = '\0';
	if (input[*i] == '-')
		(*i)++;
	while (input[*i] && !(!delim && ft_isspace((unsigned char)input[*i]))
		&& !(!delim && (is_metachar(input[*i]) && input[*i] != '-')))
	{
		if (input[*i] == delim)
			delim = '\0';
		else if (!delim && (input[*i] == '\'' || input[*i] == '"'))
			delim = input[*i];
		(*i)++;
	}
	if (*i == start)
		(*i)++;
	str = ft_strndup(input + start, *i - start);
	token = token_new(WORD, str, start);
	free(str);
	return (token);
}

static t_token	*lex_semicolon(const char *input, int *i)
{
	if (input[*i] != ';')
		return (NULL);
	return (token_new(SEMICOLON, ";", (*i)++));
}

static t_token	*lex_operator(const char *input, int *i)
{
	t_token	*t;

	t = lex_semicolon(input, i);
	if (t)
		return (t);
	t = lex_pipe_or(input, i);
	if (t)
		return (t);
	t = lex_and(input, i);
	if (t)
		return (t);
	t = lex_redir_in(input, i);
	if (t)
		return (t);
	return (lex_redir_out(input, i));
}

static t_token	*lex_tokens(const char *input, t_token *head)
{
	t_token	*cur;
	int		i;
	int		prev_i;

	cur = head;
	i = 0;
	while (*input && input[i])
	{
		prev_i = i;
		skip_whitespace(input, &i);
		cur->next = lex_operator(input, &i);
		if (!cur->next)
			cur->next = lex_word(input, &i);
		if (i == prev_i)
			return (lexer_error("lexer stuck", i, ""), free_tokens(head->next),
				NULL);
		if (cur->next && cur->next->type == ERROR)
			break ;
		if (cur->next && !validate_sequence(cur, cur->next, i, head))
			return (NULL);
		if (cur->next)
			cur = cur->next;
	}
	return (cur);
}

t_token	*lex(const char *input)
{
	static t_token	head = {0};
	t_token			*cur;
	t_token			*eof_token;

	head.type = WORD;
	cur = lex_tokens(input, &head);
	if (!cur)
		return (NULL);
	eof_token = token_new(EOF_T, NULL, 0);
	if (eof_token)
		cur->next = eof_token;
	return (head.next);
}

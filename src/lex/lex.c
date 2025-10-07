/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:16:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/30 12:27:12 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '-');
}

static void	lexer_error(const char *msg, int pos, char *token)
{
	(void)msg;
	(void)pos;
	(void)token;
	err_printf("minishell: %s %s\n", msg, "");
}

static t_token	*token_new(t_token_type type, const char *val, int pos)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = NULL;
	if (val)
		tok->value = ft_strdup(val);
	tok->pos = pos;
	tok->next = NULL;
	return (tok);
}

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

static void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] && ft_isspace((unsigned char)input[*i]))
		(*i)++;
}

static t_token	*lex_operator(const char *input, int *i)
{
	if (input[*i] == ';')
		return (token_new(SEMICOLON, ";", (*i)++));
	else if (input[*i] == '|')
	{
		if (input[*i + 1] == '|')
		{
			t_token *t = token_new(OR, "||", *i);
			*i += 2;
			return (t);
		}
		return (token_new(PIPE, "|", (*i)++));
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		t_token *t = token_new(AND, "&&", *i);
		*i += 2;
		return (t);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			t_token *t = token_new(HEREDOC, "<<", *i);
			*i += 2;
			return (t);
		}
		return (token_new(REDIR_IN, "<", (*i)++));
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			t_token *t = token_new(REDIR_APPEND, ">>", *i);
			*i += 2;
			return (t);
		}
		return (token_new(REDIR_OUT, ">", (*i)++));
	}
	return (NULL);
}

static int	validate_sequence(t_token *prev, t_token *next, int i, t_token *head)
{
	if (!next)
		return (1);
	if ((prev->type == AND || prev->type == OR)
		&& (next->type == AND || next->type == OR || next->type == PIPE))
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

t_token	*lex(const char *input)
{
	static t_token	head = {0};
	t_token			*cur;
	int				i;
	t_token			*eof_token;
	int				prev_i;

	cur = &head;
	head.type = WORD;
	i = 0;
	while (*input && input[i])
	{
		prev_i = i;
		skip_whitespace(input, &i);
		cur->next = lex_operator(input, &i);
		if (!cur->next)
			cur->next = lex_word(input, &i);
		if (i == prev_i)
		{
			lexer_error("lexer stuck", i, "");
			free_tokens(head.next);
			return (NULL);
		}
		if (cur->next && cur->next->type == ERROR)
			break ;
		if (cur->next && !validate_sequence(cur, cur->next, i, &head))
			return (NULL);
		if (cur->next)
			cur = cur->next;
	}
	eof_token = token_new(EOF_T, NULL, i);
	if (eof_token)
		cur->next = eof_token;
	return (head.next);
}

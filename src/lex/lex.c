/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:16:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 16:39:07 by chuezeri         ###   ########.fr       */
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
	fprintf(stderr, "minishell: %s %s\n", msg, "");
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

	start = *i;
	delim = '\0';
	// Allow - to be part of a word when not standalone
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
	// Safety check to prevent infinite loop
	if (*i == start)
		(*i)++;
	return (token_new(WORD, ft_strndup(input + start, *i - start), start));
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
	while (input[i])
	{
		prev_i = i;
		while (ft_isspace((unsigned char)input[i]))
			i++;
		if (input[i] == ';')
			cur->next = token_new(SEMICOLON, ";", i++);
		else if (input[i] == '|')
		{
			if (input[i + 1] == '|')
			{
				cur->next = token_new(OR, "||", i);
				i += 2;
			}
			else
				cur->next = token_new(PIPE, "|", i++);
		}
		else if (input[i] == '&' && input[i + 1] == '&')
		{
			cur->next = token_new(AND, "&&", i);
			i += 2;
		}
		else if (input[i] == '<')
		{
			if (input[i + 1] == '<')
			{
				cur->next = token_new(HEREDOC, "<<", i);
				i += 2;
			}
			else
				cur->next = token_new(REDIR_IN, "<", i++);
		}
		else if (input[i] == '>')
		{
			if (input[i + 1] == '>')
			{
				cur->next = token_new(REDIR_APPEND, ">>", i);
				i += 2;
			}
			else
				cur->next = token_new(REDIR_OUT, ">", i++);
		}
		else
			cur->next = lex_word(input, &i);
		if (i == prev_i)
		{
			lexer_error("lexer stuck", i, "");
			free_tokens(head.next);
			return (NULL);
		}
		if (cur->next && cur->next->type == ERROR)
			break ;
		if (cur->next)
		{
			if ((cur->type == AND || cur->type == OR) && (cur->next->type == AND
					|| cur->next->type == OR || cur->next->type == PIPE))
			{
				lexer_error("syntax error near unexpected token", i,
					cur->value);
				free_tokens(head.next);
				return (NULL);
			}
			if (cur->type == PIPE && cur->next->type == PIPE)
			{
				lexer_error("syntax error near unexpected token", i,
					cur->value);
				free_tokens(head.next);
				return (NULL);
			}
			cur = cur->next;
		}
	}
	eof_token = token_new(EOF_T, NULL, i);
	if (eof_token)
		cur->next = eof_token;
	return (head.next);
}

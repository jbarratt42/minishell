/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:08:55 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/17 14:40:46 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*first_quote(char *str)
{
	char	*pos1;
	char	*pos2;

	pos1 = ft_strchr(str, '\'');
	pos2 = ft_strchr(str, '"');
	if (!pos1 && !pos2)
		return (NULL);
	if (!pos1)
		return (pos2);
	if (!pos2)
		return (pos1);
	if (pos2 < pos1)
		return (pos2);
	return (pos1);
}

static t_token	*try_word(size_t size)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = malloc(size);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = WORD;
	return (token);
}

static char	*next_unquoted_space(char *str)
{
	char	delim;

	delim = '\0';
	while (*str && !(!delim && *str == ' '))
	{
		if (*str == delim)
			delim = '\0';
		if (delim == '\0' && *str == '=')
			return (str + ft_strlen(str));
		else if (!delim && (*str == '\'' || *str == '"'))
			delim = *str;
		str++;
	}
	return (str);
}

t_token	**separate_words(t_token *token)
{
	char	*p;
	char	*q;
	t_token	*next;

	next = token->next;
	p = next_unquoted_space(token->value);
	while (*p)
	{
		*p++ = '\0';
		while (*p == ' ')
			p++;
		q = next_unquoted_space(p);
		token->next = try_word(q - p + 1);
		if (!token->next)
			return (NULL);
		token = token->next;
		ft_memcpy(token->value, p, q - p);
		token->value[q - p] = 0;
		p = q;
	}
	token->next = next;
	return (&token->next);
}

void	dequote(char *str)
{
	char	*p;
	char	*q;

	str = first_quote(str);
	while (str)
	{
		p = str;
		q = ft_strchr(p + 1, *p);
		if (!q)
			return ;
		str = q - 1;
		while (*p++)
			*(p - 1) = *p;
		q--;
		while (*q++)
			*(q - 1) = *q;
		str = first_quote(str);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:50:47 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/06 11:52:38 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	compute_length(char *str, t_context *context)
{
	size_t	ret;
	char	*pos;

	ret = ft_strlen(str);
	pos = str;
	while (*pos)
	{
		if (*pos == '$')
			ret += expand_special(NULL, &pos, context);
		else
			pos++;
	}
	return (ret);
}

static void	eat_char(char **p, char **q, bool *quoted, t_context *context)
{
	if (**p == '\\')
		*(*q)++ = *(*p)++;
	else if (**p == '\'')
		*quoted = !*quoted;
	else if (!*quoted && **p == '$')
	{
		expand_special(q, p, context);
		return ;
	}
	*(*q)++ = *(*p)++;
}

/* @brief get the (length of the) new context->lineing with expanded variables
 * @param context->line context->lineing with variables
 * @param len length of new context->lineing.  if this is 0, just return the
 * length of the expanded context->lineing
 * NOTE: context->lineing must not end with '\'!
 */
char	*expand(char *str, t_context *context)
{
	bool	quoted;
	char	*ret;
	char	*p;
	char	*q;

	ret = malloc(compute_length(str, context) + 1);
	if (!ret)
		return (NULL);
	p = str;
	q = ret;
	quoted = false;
	while (*p)
		eat_char(&p, &q, &quoted, context);
	*q = '\0';
	free(str);
	return (ret);
}

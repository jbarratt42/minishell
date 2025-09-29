/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:13:10 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/29 16:33:08 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* @brief expand positional parameters
 * the positional parameters are the parameters actually passed to minishell
 * ($1, $2, etc) (not sure if these are required, holding off on confirmation)
 * UPDATE: we need to expand $0, so we might as well expand the rest.
 */
int	expand_pos_param(char **ret, char **str, t_context *context)
{
	char	tmp;
	int		i;
	int		pos;
	size_t	vallen;

	(*str)++;
	i = 0;
	while ((*str)[i] && ft_isdigit((*str)[i]))
		i++;
	tmp = (*str)[i];
	(*str)[i] = '\0';
	pos = ft_atoi(*str);
	(*str)[i] = tmp;
	*str += i;
	if (pos >= context->argc)
		return (-(i + 1));
	vallen = ft_strlen(context->argv[pos]);
	if (ret && *ret)
	{
		ft_strcpy(*ret, context->argv[pos]);
		*ret += vallen;
	}
	return (ft_strlen(context->argv[pos]) - (i + 1));
}

int	expand_status(char **ret, char **str, t_context *context)
{
	char	*tmp;
	int		len;

	tmp = ft_itoa(context->status);
	len = ft_strlen(tmp);
	(*str) += 2;
	if (ret && *ret)
	{
		ft_memcpy(*ret, tmp, len);
		*ret += len;
	}
	free(tmp);
	return (len - 2);
}

static int	pass_literal_special(char **ret, char **line)
{
	if (ret && *ret)
		*(*ret)++ = *(*line - 1);
	return (0);
}

/* @brief copy the value of the variable pointed to in str to ret and advance
 *  both
 * @param ret a p2ptr to the beginning of the expanded string. if *ret == NULL,
 * 	just return the difference between the value and varname lengths
 * 	(including '$')
 * @param str a p2ptr to the '$' character before the variable name
 */
int	expand_variable(char **ret, char **line, t_context *context)
{
	size_t	len;
	char	tmp;
	char	*val;

	(*line)++;
	len = 0;
	while ((*line)[len] && (ft_isalnum((*line)[len]) || (*line)[len] == '_'))
		len++;
	if (!len)
		return (pass_literal_special(ret, line));
	tmp = (*line)[len];
	(*line)[len] = '\0';
	val = ft_getenv(*line, context->env);
	if (!val)
		val = ft_getenv(*line, context->local);
	(*line)[len] = tmp;
	*line += len;
	if (!val)
		return (-(len + 1));
	if (ret && *ret)
	{
		ft_strcpy(*ret, val);
		*ret += ft_strlen(val);
	}
	return (ft_strlen(val) - (len + 1));
}

int	expand_special(char **ret, char **line, t_context *context)
{
	if (*(*line + 1) == '?')
		return (expand_status(ret, line, context));
	if (ft_isdigit(*(*line + 1)))
		return (expand_pos_param(ret, line, context));
	if (ft_isalpha(*(*line + 1)) || *(*line + 1) == '_')
		return (expand_variable(ret, line, context));
	if (ret && *ret)
		*(*ret)++ = *(*line)++;
	else
		(*line)++;
	return (0);
}

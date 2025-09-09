/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:13:10 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/09 09:55:57 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* @brief expand positional parameters
 * the positional parameters are the parameters actually passed to minishell
 * ($1, $2, etc) (not sure if these are required, holding off on confirmation)
 * UPDATE: we need to expand $0, so we might as well expand the rest.
 */
int expand_pos_param(char **ret, char **str, t_context *context)
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
	(*str)[i] = '\0';				  // null-terminate tcohe position string.
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

char *ft_getenv(char *name, char **env)
{
	const size_t len = ft_strlen(name);

	while (*env)
	{
		if (!ft_strncmp(name, *env, len) && (*env)[len] == '=')
			return (*env + len + 1);
		env++;
	}
	return (NULL);
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
int expand_variable(char **ret, char **line, char **env)
{
	size_t len;
	char tmp;
	char *val;

	(*line)++;
	(void)env;
	len = 0;
	while ((*line)[len] && (ft_isalnum((*line)[len]) || (*line)[len] == '_'))
		len++;
	if (!len)
		return (pass_literal_special(ret, line));
	tmp = (*line)[len];
	(*line)[len] = '\0'; // null-terminate the variable name
	val = ft_getenv(*line, env);
	(*line)[len] = tmp;
	*line += len;
	if (!val) // NULL env vars get expanded to empty string
		return (-(len + 1));
	if (ret && *ret)
	{
		ft_strcpy(*ret, val);
		*ret += ft_strlen(val);
	}
	return (ft_strlen(val) - (len + 1));
}

int expand_history(char **str, char **s)
{
	(void)str;
	(void)s;
	return 0;
}

int expand_special(char **ret, char **line, t_context *context)
{
	// char tmp;
	// int i;
	// char *val;
	// size_t varlen;

	//if (**line == '!')
	//	return (expand_history(ret, line));
	// *line == '$'
	if (ft_isdigit(*(*line + 1)))
		return (expand_pos_param(ret, line, context));
	return (expand_variable(ret, line, context->env));
}

/*
static char *try_new_line(size_t len, t_context *context)
{
	char *ret;

	ret = malloc(len);
	if (!ret)
	{
		free_context(context);
		perror("try_new_line");
		exit(1);
	}
	return (ret);
}
*/

static size_t	compute_length(t_context *context)
{
	size_t	ret;
	char	*pos;

	ret = ft_strlen(context->input);
	pos = context->input;
	while (*pos)
	{
		if (*pos == '$')
			ret += expand_special(NULL, &pos, context);
		else
			pos++;
	}
	return (ret);
}

/* @brief get the (length of the) new context->lineing with expanded variables
 * @param context->line context->lineing with variables
 * @param len length of new context->lineing.  if this is 0, just return the
 * length of the expanded context->lineing
 * NOTE: context->lineing must not end with '\'!
 */
bool	expand(t_context *context)
{
	bool quoted;
	char *ret;
	char *p;
	char *q;

	ret = malloc(compute_length(context) + 1);
	if (!ret)
		return (false);
	p = context->input;
	q = ret;
	quoted = false;
	while (*p)
	{
		if (*p == '\\')
			*q++ = *p++;
		else if (*p == '\'')
			quoted = !quoted;
		//else if (!quoted && (*line == '$' || *line == '!'))
		else if (!quoted && *p == '$')
		{
			expand_special(&q, &p, context);
			continue;
		}
		*q++ = *p++;
	}
	*q = '\0';
	free(context->input);
	context->input = ret;
	return (true);
}

/* a valid input string does not end with '\' and has no unclosed quotes
 * and no unclosed unquoted parentheses
 */
bool is_valid_input(char *str)
{
	bool quoted;
	bool qquoted;
	int level;

	quoted = false;
	qquoted = false;
	level = 0;
	while (*str)
	{
		if (*str == '"' && !quoted)
			qquoted = !qquoted;
		if (*str == '\'' && !qquoted)
			quoted = !quoted;
		if (*str == '\\' && !quoted)
		{
			if (!*(str + 1)) // escaped NULL terminator
				return (false);
			str += 2;
			continue;
		}
		if (*str == '{' && (!quoted && !qquoted))
			level++;
		if (*str == '}' && (!quoted && !qquoted))
			level--;
		if (level < 0)
			return (false);
		str++;
	}
	if (level != 0 || (quoted || qquoted)) // invalid parens or unclosed quote
		return (false);
	return (true);
}

/* @brief expand all the variables in str and return the new string.
 * first call count_or_expand to get the new length of the string,
 * then call count_or_expand again to get the new string, freeing the old.
 * @param str input string
 * @param argc number of arguments to minishell
 * @param argv argument array from main
 */
/*
char *expand_all(t_context *context)
{
	int len = 0;
	char *new = NULL;

	if (!*context->line) // str is empty
		return NULL;
	if (!is_valid_input(context->line))
	{
		free_context(context);
		perror("invalid input string");
		return (NULL);
	}
	len = count_or_expand(len, context);
	count_or_expand(len, context);
	return (new);
}
*/

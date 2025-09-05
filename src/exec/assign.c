#include "minishell.h"

t_token	*delete_tokens(t_token *begin, int num)
{
	t_token **tail;
	t_token *ret;

	tail = &begin;
	while (num--)
		tail = *tail->next;
	ret = *tail;
	*tail = NULL;
	free_token(begin);
	return (ret);
}



char	**assign(t_token **tokens, t_context context)
{
	char	**env;
	t_token	**token;
	t_token	*tmp;

	token = tokens;
	env = copy_env(context->env);
	while (**token && !is_command(*token))
	{
		if (*token->type == WORD && ft_strchr(token->value, '='))
		{
			env = set_env(token->value, env);
			if (!env)
			{
				perror("assign");
				return (NULL);
			}
			*token = delete_tokens(*token, 1);
		}
		else
			*token = *token->next;
	}
}

bool	reassign_fd(t_token *token, context)
{
	const int	fd = toke->type > REDIR_OUT;
	int			mode;

	if (token->type == REDIR_IN)
		mode = O_RDONLY;
	else if (token->type == REDIR_OUT)
		mode = O_WRONLY;
	else if (token->type == REDIR_APPEND)
		mode = O_APPEND;
	else
		return (heredoc(token, context))
	if(!try_close(context->open[fd]))
		return (false);
	context->open[fd] = open(token->next->value, mode);
	if (!context->open[fd])
		return (false);
	return (true);
}

bool	redirect(t_token **tokens, t_context context)
{
	t_token **token;
	t_token *tmp;
	
	token = tokens;
	while (**token)
		if(*token->type >= REDIR_IN && *token->type <= REDIR_APPEND)
		{
			if(!reassign_fd(token, context))
				return (false);
			*token = delete_tokens(*token, 2);
		}
		else
			*token = *token->next;
	return (true);
}

t_token	*relex(t_token *token)
{
	char 			*pos;
	const t_token	*next = token->next;
	const t_token	*head = token;
	size_t			len;

	pos = str_chr(token->value, ' ')
	while (pos)
	{
		*pos++ = '\0';
		while (*pos == ' ')
			pos++;
		token->next = malloc(sizeof(t_token));
		if (!token->next)
		{
			free_token(head);
			return (NULL);
		}
		token = token->next;
		token->value = ft_strdup(pos);
		if (!token->value)
		{
			free_token(head);
			return (NULL);
		}
		pos = str_chr(token->value, ' ');
	}
	token->next = next;
	return (next);
}

bool	expand_tokens(t_token **tokens, t_context context)
{
	t_token **token;
	bool	squoted;
	bool	dquoted;

	squoted = false;
	dquoted = false;
	token = tokens;
	while (*token)
		if (*token->type == WORD && !squoted)
		{
			expand(*token, context);
			if (!dquoted)
				*token = relex(*token);
		}
		else if (*token->type == SQUOTE || *token->type == DQUOTE)
		{
			if (*token->type == SQUOTE)
				squoted = !squoted;
			if (*token->type == DQUOTE)
				dquoted = !dquoted;
			*token = delete_tokens(*token, 1);
		}
		else
			*token = *token->next;
}

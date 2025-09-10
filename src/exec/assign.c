#include "minishell.h"

/* delete len tokens starting with the token pointed to by token */
void	delete_tokens(t_token **token, size_t len)
{
	t_token			*tmp;
	t_token			*end;

	tmp = *token;
	if (!len)
		return ;
	end = *token;
	while (len--)
		end = end->next;
	*token = end;
	end = NULL;
	free (tmp);
}

/* check if a token list contains a command */
bool	is_command(t_token *token)
{
	while (token && token->type != EOF_T)
		if (token->type >= REDIR_IN && token->type <= REDIR_APPEND)
			token = token->next->next;
		else if (token->type == WORD && ft_strchr(token->value, '='))
			token = token->next;
		else
			return (true);
	return (false);
}
		
/* implement and delete assignment tokens from left to right */
bool	assign(t_token **token, t_context *context)
{
	while (*token && (*token)->type != EOF_T)
		if ((*token)->type == WORD && ft_strchr((*token)->value, '='))
		{
			context->local = set_env((*token)->value, context->local);
			if(!context->local)
				return (false);
			delete_tokens(token, 1);
		}
		else
			token = &((*token)->next->next);
	return (true);
}

/* process a heredoc */
bool	heredoc(t_token *token, t_context *context)
{
	(void)token;
	(void)context;
	return (true);
}

/* open the file in token->next->value, setting the file desciptor in 
 * context open, and closing fds as required */
bool	reassign_fd(t_token *token, t_context *context)
{
	const int	fd = token->type > REDIR_OUT;
	int			mode;

	if (token->type == REDIR_IN)
		mode = O_RDONLY;
	else if (token->type == REDIR_OUT)
		mode = O_WRONLY;
	else if (token->type == REDIR_APPEND)
		mode = O_APPEND;
	else
		return (heredoc(token, context));
	if(context->open[fd] > 2 && close(context->open[fd]) == -1)
		return (false);
	context->open[fd] = open(token->next->value, mode);
	if (!context->open[fd])
		return (false);
	return (true);
}

/* redirect file descriptors left to right and delete the corresponding tokens*/
bool	redirect(t_token **token, t_context *context)
{
	while (*token)
		if((*token)->type >= REDIR_IN && (*token)->type <= REDIR_APPEND)
		{
			if(!reassign_fd(*token, context))
				return (false);
			delete_tokens(token, 2);
		}
		else
			token = &((*token)->next);
	return (true);
}

static t_token	*try_word(size_t size)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = malloc(size);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

static char	*next_unquoted_space(char *str)
{
	char	delim;
	bool	quoted;

	delim = '\0';
	quoted = false;
	while (*str && !(!quoted && *str == ' '))
	{
		if (*str == '\'' || *str == '"')
		{
			if (!quoted)
			{
				delim = *str;
				quoted = true;
			}
			if (quoted && delim == *str)
				quoted = false;
		}
		str++;
	}
	return (str);
}


t_token	**separate_words(t_token *token)
{
	char 	*p;
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
		token->value[q-p] = 0;
		p = q;
	}
	token->next = next;
	return (&token->next);
}

/* expand variables in tokens */
bool	expand_tokens(t_token **token, t_context *context)
{
	bool	squoted;
	bool	dquoted;

	squoted = false;
	dquoted = false;
	while (*token && (*token)->type < PIPE && (*token)->type != EOF_T)
		if ((*token)->type == WORD && !squoted)
		{	
			(*token)->value = expand((*token)->value, context);
			if(!*token)
				return (false);
			if (!dquoted)
				token = separate_words(*token);
		}
		else if ((*token)->type == SQUOTE || (*token)->type == DQUOTE)
		{
			if ((*token)->type == SQUOTE)
				squoted = !squoted;
			if ((*token)->type == DQUOTE)
				dquoted = !dquoted;
			delete_tokens(token, 1);
		}
		else
			token = &((*token)->next);
	return (true);
}

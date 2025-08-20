#include "minishell.h"

/*
enum e_toktype token_type(t_token *token)
{
	if (*token->txt == '(' || *token->txt == '{')
		return (OPEN);
	if (*token->txt == ')' || *token->txt == '}')
		return (CLOSE);
	if (*token->txt == '|' && !*(token->txt + 1))
		return (PIPE);
	if (*token->txt == '&' && *(token->txt + 1) == '&')
		return (AND);
	if (*token->txt == '|' && *(token->txt + 1) == '|')
		return (OR);
	if (*token->txt == ';')
		return (SEMICOLON);
	return (WORD);
}
*/

/* take a pointer to the beginning of a token in a string and extract the whole 
 * token.  If the first character is a quote, the whole quoted strding is 
 * the token, otherwise the _word_ is the token
 */
t_token *extract_token(char *s, size_t size)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("extract_token(token)");
		return (NULL);
	}
	token->txt = malloc(size + 1);
	if (!token->txt)
	{
		perror("extract_token(token->txt)")
		free_token(token);
		return (NULL);
	}
	process_token(s, size, *token);
	token->txt[size] = 0;
	return (token);
}

char	*consume_whitespace(char *p)
{
	while (*p && (*p == ' ' || *p == '\t'))
		p++;
	return (p);
}

static bool	is_special(char c)
{
	return(c == '&' 
			|| c == '|'
			|| c == ';'
			|| c == '<'
			|| c == '>');
}

/* @brief expand variables and split into tokens
 * @param s the input string (including \n for multiline)
 * @param delim ' ' for outermost invocation, '"' and '\'' for quoted token
 */
/*
t_token *lex(char *s, char delim, t_context *context)
{
	char	*p;
	char	*q;
	t_token *token;

	p = s;
	q = p;
	while (*q && *q != delim && (delim == ' ' && !is_special(*q)))
		q++;
	if (!*q && delim != ' ')
	{
		perror("unclosed quote");
		exit(1);
	}
	token = extract_token(p, q - p);
	if (!token)
		return (NULL);
	token->delim = delim;
	q = consume_whitespace(q);
	if (*q)
		if (*q == '\'' || *q == '"')
			token->next = lex(q + 1, *q);
		else
			token->next = lex(q, ' ');
	else
		token->next = NULL;
	return (token);
}
*/

static char	*try_dup_txt(t_token *token, char *s, size_t len)
{
	token->txt = malloc(len + 1);
	if (!token->txt)
	{
		perror("try_dup_txt");
		free_token(token);
		return (NULL);
	}
	ft_strlcpy(token->txt, s, len + 1);
	return (p);
}

char	*consume_quote(char *s, t_token *token)
{
	size_t		len;
	const char	delim = *s++;

	len = 0;
	while (s[len] && s[len] != delim)
		len++;
	if (!s[len])
	{
		perror("unclosed quote");
		free_token(token);
		return (NULL);
	}
	if (!try_dup_txt(token, s, len))
		return (NULL);
	token->type = WORD;
	return (s + len + 1);
}

char	*consume_word(char *s, t_token *token)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != ' ' && !is_special(s[len]))
		if (s[len] == '\\')
			len++;
		len++;
	if (!try_dup_txt(token, s, len))
		return (NULL);
	return (s + len);
}

char	*consume_redirect(char *s, t_token *token)
{
	size_t 		len;
	e_toktype	type;
	
	if (*s == '<' && *(s + 1) == '<')
		return (consume_heredoc(s, token));
	type = FILE_IN;
	while (*s++ == '>')
		type++;
	s = consume_whitespace(s);
	if (*s == '"' || *s == '\'')
		s = consume_quote(s, token);
	else
		s = consume_word(s, token);
	token->type = type;
	return (s);
}

char	*consume_operator(char *s, t_token *token)
{
	token->txt = NULL;
	if (*s == '&' && *(s + 1) == '&')
	{
		token->type = AND;
		return (s + 2);
	}
	if (*s == '|' && *(s + 1) == '|')
	{
		token->type = OR;
		return (s + 2);
	}
	if (*s == '|' && *(s + 1) != '|')
	{
		token->type = PIPE;
		return (s + 1);
	}
	perror("unknown operator");
	return (NULL);
}

t_token *lex(t_context	*context)
{
	t_token *const token = malloc(sizeof(t_token));
	if (!token)
		return (null);
	init_t
	if (*s == '"' || *s == '\'')
		s = consume_quote(s, token);
	else if (*s == '<' || *s == '>')
		s = consume_redirect(s, token);
	else if (*s == '|' || (*s == '&' && *(s + 1) == '&'))
		s = consume_operator(s, token);
	else
		s = consume_word(s, token);
	if (!s)
		return (NULL);
	s = consume_whitespace(s);
	token->next = lex(s, context);
}

int print_tokens(t_token *token)
{
	int len = 0;
	while (token)
	{
		len += printf("%s ", token->txt);
		token = token->next;
	}
	return (len);
}

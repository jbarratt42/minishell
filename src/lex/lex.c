/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:16:35 by chuezeri          #+#    #+#             */
/*   Updated: 2025/08/24 19:43:02 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_metachar(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == '&' || c == '-');
}

static void lexer_error(const char *msg, int pos, char *token)
{
    (void)msg;
    (void)pos;
    // perror("minishell");
    fprintf(stderr, "minishell: %s %s\n", msg, token);
}

static t_token *token_new(t_token_type type, const char *val, int pos)
{
    t_token *tok = malloc(sizeof(t_token));
    if (!tok)
        return NULL;
    tok->type = type;
    tok->value = val ? ft_strdup(val) : NULL;
    tok->pos = pos;
    tok->next = NULL;
    return tok;
}

static t_token *lex_word(const char *input, int *i)
{
    int start = *i;
    // Allow - to be part of a word when not standalone
    if (input[*i] == '-')
        (*i)++;
    while (input[*i] && !ft_isspace((unsigned char)input[*i]) &&
           !(is_metachar(input[*i]) && input[*i] != '-'))
        (*i)++;
    return (token_new(WORD, ft_strndup(input + start, *i - start), start));
}

static t_token *lex_quote(const char *input, int *i, char quote)
{
    int start = ++(*i);
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (!input[*i])
    {
        lexer_error("unclosed quote", start - 1, NULL);
        return token_new(ERROR, NULL, start - 1);
    }

    // Create opening quote token
    t_token *head = token_new(
        quote == '\'' ? SQUOTE : DQUOTE,
        ft_strndup(&quote, 1),
        start - 1);

    // Create content token if there's content inside
    if (*i > start)
    {
        head->next = token_new(WORD, ft_strndup(input + start, *i - start), start);
        head->next->prev = head;
    }

    // Create closing quote token
    t_token *closing_quote = token_new(
        quote == '\'' ? SQUOTE : DQUOTE,
        ft_strndup(&quote, 1),
        *i);

    if (head->next)
    {
        head->next->next = closing_quote;
        closing_quote->prev = head->next;
    }
    else
    {
        head->next = closing_quote;
        closing_quote->prev = head;
    }

    (*i)++; // skip closing quote
    return head;
}

t_token *lex(const char *input)
{
    t_token head = {0};
    t_token *cur = &head;
    head.type = WORD; // Initialize the dummy head node with a valid type

    int i = 0;

    while (input[i])
    {
        if (ft_isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }

        if (input[i] == '\'' || input[i] == '"')
        {
            t_token *quote_tokens = lex_quote(input, &i, input[i]);
            if (quote_tokens)
            {
                cur->next = quote_tokens;
                // Find the last token in the quote sequence
                while (quote_tokens->next)
                    quote_tokens = quote_tokens->next;
                cur = quote_tokens;
            }
        }
        else if (input[i] == ';')
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
        // Always ensure we make progress to avoid infinite loop
        if (cur->next && cur->next->type == ERROR)
            break;

        if (cur->next)
        {
            // check if prev is and, or, pipe, redir, heredoc if so its invalid
            if (cur->type == AND || cur->type == OR || cur->type == PIPE ||
                cur->type == REDIR_IN || cur->type == REDIR_OUT || cur->type == HEREDOC)
            {

                if (cur->next->type == AND || cur->next->type == OR || cur->next->type == PIPE ||
                    cur->next->type == REDIR_IN || cur->next->type == REDIR_OUT || cur->next->type == HEREDOC)
                {

                    lexer_error("syntax error near unexpected token", i, cur->value);
                    free_tokens(head.next);
                    return NULL;
                }
            }
            cur = cur->next;
        }
    }
    // Add EOF token at the end with proper type
    t_token *eof_token = token_new(EOF_T, NULL, i);
    if (eof_token)
        cur->next = eof_token;

    return (head.next);
}

void free_tokens(t_token *tok)
{
    t_token *tmp;
    while (tok)
    {
        tmp = tok->next;
        if (tok->value)
            free(tok->value);
        free(tok);
        tok = tmp;
    }
}

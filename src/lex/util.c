/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:50:47 by chuezeri          #+#    #+#             */
/*   Updated: 2025/08/24 19:38:36 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char *token_type_str(t_token_type type)
{
    switch (type)
    {
    case WORD:
        return "WORD";
    case SQUOTE:
        return "SQUOTE";
    case DQUOTE:
        return "DQUOTE";
    case PIPE:
        return "PIPE";
    case AND:
        return "AND";
    case OR:
        return "OR";
    case SEMICOLON:
        return "SEMICOLON";
    case REDIR_IN:
        return "REDIR_IN";
    case REDIR_OUT:
        return "REDIR_OUT";
    case REDIR_APPEND:
        return "REDIR_APPEND";
    case HEREDOC:
        return "HEREDOC";
    case EOF_T:
        return "EOF";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

void print_tokens(const t_token *tok)
{
    printf("|-------------TOKENS----------------|\n");
    printf("|-----------------------------------|\n");
    printf("| %-12s | %-5s | %-10s |\n", "Type", "Pos", "Val");
    printf("|--------------|-------|------------|\n");

    int count = 0;
    while (tok)
    {
        printf("| %-12s | %-5d | %-10s |\n",
               token_type_str(tok->type),
               tok->pos,
               tok->value ? tok->value : "(null)");

        printf("|--------------|-------|------------|\n");
        tok = tok->next;
        count++;
    }

    printf("Total tokens: %d\n", count);
}

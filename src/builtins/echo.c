/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:46:42 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 12:48:03 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_n_flag(const char *value)
{
    int		index;

    if (!value || value[0] != '-')
        return (false);
    index = 1;
    if (value[index] != 'n')
        return (false);
    while (value[index] == 'n')
        index++;
    return (value[index] == '\0');
}

static t_token	*consume_n_flags(t_token *current, bool *newline)
{
    while (current && current->type == WORD && is_n_flag(current->value))
    {
        *newline = false;
        current = current->next;
    }
    return (current);
}

static void	print_words(t_token *current)
{
    while (current && current->type == WORD)
    {
        ft_printf("%s", current->value);
        current = current->next;
        if (current && current->type == WORD)
            ft_printf(" ");
    }
}

int	builtin_echo(t_token *tokens, t_context *context)
{
    bool	newline;
    t_token	*current;

    (void)context;
    newline = true;
    current = tokens;
    if (current && current->type == WORD)
        current = current->next;
    current = consume_n_flags(current, &newline);
    print_words(current);
    if (newline)
        ft_printf("\n");
    return (0);
}

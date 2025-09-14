/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:10:26 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/13 08:43:05 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        rl_replace_line("", 0);
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_redisplay();
        // Don't set rl_done = 1 here as it causes readline to exit
        // The signal should just clear the current line and continue
    }
    else if (sig == SIGTERM)
    {
        printf("Terminated by SIGTERM\n");
        g_status = SIGTERM;
        exit(0);
    }
}

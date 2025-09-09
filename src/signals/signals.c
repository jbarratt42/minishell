/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:10:26 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/09 10:57:38 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handler(int sig, siginfo_t *info, void *ucontext)
{

    (void)ucontext; // not used
    printf("Caught signal %d from PID %d\n", sig, info->si_pid);

    if (sig == SIGINT)
    {
        g_status = SIGINT;
    }
    else if (sig == SIGQUIT)
    {
        // Handle Ctrl+
        printf("\n");
    }
    else if (sig == SIGTERM)
    {
        g_status = SIGTERM;
        exit(0);
    }
}

void handle_signals(t_context *context)
{

    static struct sigaction sa = {0};
    sa.sa_flags = 0; // use 3-arg handler
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);

    int a = sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Signal handlers set up (sigaction returned %d)\n", a);

    // context.
    (void)context;
}

void signal_handler(int sig)
{

    if (sig == SIGINT)
    {

        rl_replace_line("", 0);
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_redisplay();
        rl_done = 1;
    }
    else if (sig == SIGTERM)
    {
        g_status = SIGTERM;
        exit(0);
    }
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:10:26 by chuezeri          #+#    #+#             */
/*   Updated: 2025/11/12 14:19:29 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		g_status = SIG_DEFAULT + SIGINT;
	}
//	else if (sig == SIGTERM)
//	{
//		ft_printf("Terminated by SIGTERM\n");
//		g_status = SIGTERM;
//		exit(0);
//	}
}

void	execve_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_status = SIG_DEFAULT + SIGINT;
	}
//	else if (sig == SIGTERM)
//	{
//		ft_printf("Terminated by SIGTERM\n");
//		g_status = SIGTERM;
//		exit(0);
//	}
}

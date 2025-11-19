/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:45:07 by jbarratt          #+#    #+#             */
/*   Updated: 2025/11/19 13:37:49 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	collect(int pid)
{
	int	w_status;

	if (waitpid(pid, &w_status, 0) == -1)
	{
		perror("collect");
		return (-1);
	}
	/*
	ft_printf("pid %d\nraw %d\nexited %d\nsignaled %d\ntermsig %d\nstatus %d\n",
			pid, w_status, WIFEXITED(w_status), WIFSIGNALED(w_status), 
			WTERMSIG(w_status), WEXITSTATUS(w_status));
			*/
	return (w_status);
}

int	collect2(int pids[2])
{
	int	status[2];
	int	i;

	i = 1;
	while (i >= 0)
	{
		if (pids[i] && pids[i] != -1)
		{
			status[i] = collect(pids[i]);
			if (status[i] == -1)
				return (-1);
		}
		else
			status[i] = 0;
		i--;
	}
	return (status[1]);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:10:03 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/03 11:34:18 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	try_close2(int open[2])
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (open[i] > 2)
		{
			if (close(open[i]) == -1)
			{
				perror("try_close2");
				return (false);
			}
		}
		i++;
	}
	return (true);
}

bool	try_pipe(int fds[2])
{
	int	tmp[2];

	if (pipe(tmp) == -1)
	{
		perror("try_pipe");
		return (false);
	}
	fds[0] = tmp[1];
	fds[1] = tmp[0];
	return (true);
}

bool	try_dup2(int open[3])
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (open[i] != i)
			if (dup2(open[i], i) == -1)
				return (false);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		if (open[i] > 2)
			if (close(open[i]) == -1)
				return (false);
		i++;
	}
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:37:23 by jbarratt          #+#    #+#             */
/*   Updated: 2024/12/09 14:41:42 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

int	ft_dprintf_char(int fd, char c)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_dprintf_str(int fd, char *s)
{
	int	i;

	if (!s)
		return (ft_dprintf_str(fd, "(null)"));
	i = 0;
	while (s[i])
		ft_dprintf_char(fd, s[i++]);
	return (i);
}

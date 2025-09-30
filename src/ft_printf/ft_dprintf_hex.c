/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_hex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:35:34 by jbarratt          #+#    #+#             */
/*   Updated: 2024/12/09 14:12:40 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_dprintf_hex(int fd, unsigned long n, int is_upper)
{
	const char	base_lower[] = "0123456789abcdef";
	const char	base_upper[] = "0123456789ABCDEF";
	char		*base;

	if (is_upper)
		base = (char *)base_upper;
	else
		base = (char *)base_lower;
	return (ft_dprintf_base(fd, n, 0, base));
}

int	ft_dprintf_ptr(int fd, void *p)
{
	const unsigned long	lp = (unsigned long)p;

	if (!p)
		return (ft_dprintf_str(fd, "(nil)"));
	ft_dprintf_str(fd, "0x");
	return (ft_dprintf_hex(fd, lp, 0) + 2);
}

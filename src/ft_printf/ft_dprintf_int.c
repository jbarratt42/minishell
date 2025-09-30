/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_int.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:40:30 by jbarratt          #+#    #+#             */
/*   Updated: 2024/12/06 11:56:35 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_dprintf_base(int fd, unsigned long n, size_t len, char *base)
{
	int					r;
	const unsigned long	div = ft_strlen(base);

	if (n > div - 1)
		r = ft_dprintf_base(fd, n / div, len, base) + 1;
	else
		r = 1;
	ft_dprintf_char(fd, base[n % div]);
	return (r);
}

int	ft_dprintf_unsigned_int(int fd, unsigned long n)
{
	return (ft_dprintf_base(fd, n, 0, "0123456789"));
}

int	ft_dprintf_int(int fd, int n)
{
	int	len;

	if (n == INT_MIN)
		return (ft_dprintf_str(fd, "-2147483648"));
	len = 0;
	if (n < 0)
	{
		ft_dprintf_char(fd, '-');
		n = -n;
		len++;
	}
	return (ft_dprintf_unsigned_int(fd, n) + len);
}

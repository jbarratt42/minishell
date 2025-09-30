/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_double.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:42:54 by jbarratt          #+#    #+#             */
/*   Updated: 2024/12/04 11:37:16 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_dprintf_padded_int(int fd, int n, size_t width, char pad)
{
	int	m;
	int	len;

	len = 0;
	if (n < 0)
	{
		if (n == INT_MIN)
			return (ft_dprintf_str(fd, "-2147483648"));
		n = -n;
		ft_dprintf_char(fd, '-');
		width--;
		len++;
	}
	m = n;
	width--;
	while (width > 0)
	{
		m /= 10;
		width--;
		if (m == 0)
			len += ft_dprintf_char(fd, pad);
	}
	return (len + ft_dprintf_int(fd, n));
}

int	ft_dprintf_double(int fd, double x, size_t prec)
{
	int	i;
	int	len;

	len = ft_dprintf_int(fd, (int)x);
	len += ft_dprintf_char(fd, '.');
	x -= (int)x;
	if (x < 0)
		x = -x;
	i = 0;
	while (i < (int)prec)
	{
		x *= 10;
		i++;
	}
	if (x - (int)x > 0.5
		|| (x - (int)x == 0.5
			&& (int)x % 2))
		x++;
	len += ft_dprintf_padded_int(fd, (int)x, prec, '0');
	return (len);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 08:51:02 by jbarratt          #+#    #+#             */
/*   Updated: 2024/12/09 14:40:36 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_dprintf_arg(int fd, va_list args, char **c)
{
	if (**c == '%')
		return (ft_dprintf_char(fd, '%'));
	else if (**c == 'u')
		return (ft_dprintf_unsigned_int(fd, va_arg(args, unsigned int)));
	else if (**c == 'c')
		return (ft_dprintf_char(fd, (char)va_arg(args, int)));
	else if (**c == 's')
		return (ft_dprintf_str(fd, va_arg(args, char *)));
	else if (**c == 'f')
		return (ft_dprintf_double(fd, va_arg(args, double), 6));
	else if (**c == 'x')
		return (ft_dprintf_hex(fd, va_arg(args, unsigned int), 0));
	else if (**c == 'X')
		return (ft_dprintf_hex(fd, va_arg(args, unsigned int), 1));
	else if (**c == 'i' || **c == 'd')
		return (ft_dprintf_int(fd, va_arg(args, int)));
	else if (**c == 'p')
		return (ft_dprintf_ptr(fd, va_arg(args, void *)));
	else
		return (-1);
}

int	ft_vdprintf(int fd, const char *format, va_list args)
{
	char	*c;
	int		len;
	int		inc;

	len = 0;
	c = (char *)format;
	while (*c)
	{
		if (*c == '%')
		{
			c++;
			inc = ft_dprintf_arg(fd, args, &c);
			if (inc == -1)
				return (-1);
			len += inc;
		}
		else
		{
			ft_dprintf_char(1, *c);
			len++;
		}
		c++;
	}
	return (len);
}

int	ft_printf(const char *format, ...)
{
	int		len;
	va_list	args;

	va_start(args, format);
	len = ft_vdprintf(1, format, args);
	va_end(args);
	return (len);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:31:08 by jbarratt          #+#    #+#             */
/*   Updated: 2024/12/12 10:51:38 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <limits.h>
# include <unistd.h>
# include <stddef.h>

size_t	ft_strlen(const char *s);
int		ft_dprintf_char(int fd, char c);
int		ft_dprintf_str(int fd, char *s);
int		ft_dprintf_base(int fd, unsigned long n, size_t len, char *base);
int		ft_dprintf_unsigned_int(int fd, unsigned long n);
int		ft_dprintf_int(int fd, int n);
int		ft_dprintf_hex(int fd, unsigned long n, int is_upper);
int		ft_dprintf_ptr(int fd, void *p);
int		ft_dprintf_padded_int(int fd, int n, size_t width, char pad);
int		ft_dprintf_double(int fd, double x, size_t prec);
int		ft_dprintf_arg(int fd, va_list args, char **c);
int		ft_vdprintf(int fd, const char *format, va_list args);
int		ft_printf(const char *format, ...);

#endif

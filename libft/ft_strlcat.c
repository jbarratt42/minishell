/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:45:18 by chuezeri          #+#    #+#             */
/*   Updated: 2024/11/14 16:20:52 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	count;
	size_t	length;
	size_t	dest_length;
	size_t	src_length;

	count = 0;
	length = ft_strlen(dest);
	dest_length = ft_strlen(dest);
	src_length = ft_strlen(src);
	if (size <= dest_length)
		return (size + src_length);
	while (src[count] && count < size - dest_length - 1)
	{
		dest[length + count] = src[count];
		count++;
	}
	dest[length + count] = '\0';
	return (dest_length + src_length);
}

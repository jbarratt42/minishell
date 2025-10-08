/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:55:22 by chuezeri          #+#    #+#             */
/*   Updated: 2024/11/16 09:09:56 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcat(char *dest, const char *src)
{
	size_t	count;
	size_t	lenght;

	count = 0;
	lenght = ft_strlen(dest);
	while (src[count])
	{
		dest[lenght + count] = src[count];
		count++;
	}
	dest[lenght + count] = '\0';
	return (dest);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	len;
	char	*dest;

	len = ft_strlen(s1) + ft_strlen(s2);
	dest = (char *)malloc(sizeof(char) * len + 1);
	if (!dest)
		return (dest);
	dest[0] = '\0';
	ft_strcat(dest, s1);
	ft_strcat(dest, s2);
	return (dest);
}

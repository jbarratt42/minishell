/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:31:32 by chuezeri          #+#    #+#             */
/*   Updated: 2024/11/18 18:03:56 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	size_t		i;
	size_t		j;
	size_t		len;

	i = 0;
	len = ft_strlen((char *)needle);
	if (haystack == needle || !len)
		return ((char *)haystack);
	while (haystack[i] && i < n)
	{
		j = 0;
		while ((n - i >= len) && haystack[j + i] && needle[j] && haystack[i
				+ j] == needle[j])
			j++;
		if (needle[j] == '\0')
			return ((char *)&haystack[i]);
		i++;
	}
	return (0);
}

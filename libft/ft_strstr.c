/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:46:04 by chuezeri          #+#    #+#             */
/*   Updated: 2024/11/23 15:45:56 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *str, const char *to_find)
{
	int	i;
	int	find_len;
	int	count;

	i = 0;
	find_len = ft_strlen((char *)to_find);
	if (str == to_find || !find_len)
		return ((char *)str);
	while (str[i])
	{
		count = 0;
		while (str[count + i] && to_find[count]
			&& str[i + count] == to_find[count])
			count++;
		if (to_find[count] == '\0')
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}

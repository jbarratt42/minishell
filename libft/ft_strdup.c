/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strdup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:53:41 by chuezeri          #+#    #+#             */
/*   Updated: 2024/11/08 09:53:42 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	int		lenght;
	char	*dest;

	lenght = 0;
	while (s[lenght])
		lenght++;
	dest = (char *)malloc(sizeof(char) * (lenght + 1));
	if (!dest)
		return (NULL);
	while (lenght >= 0)
	{
		dest[lenght] = s[lenght];
		lenght--;
	}
	return (dest);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:45:59 by chuezeri          #+#    #+#             */
/*   Updated: 2024/11/22 18:45:19 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	is_separator(char c, char sep)
{
	if (c == sep)
		return (1);
	return (0);
}

static int	count_words(char *str, char charset)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (!is_separator(*str, charset) && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (is_separator(*str, charset))
			in_word = 0;
		str++;
	}
	return (count);
}

static char	*extract_word(char *str, char charset)
{
	int		i;
	int		len;
	char	*word;

	len = 0;
	while (str[len] && !is_separator(str[len], charset))
		len++;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static void	*ft_freeall(char **str, int i)
{
	while (i >= 0)
	{
		free(str[i]);
		i--;
	}
	free(str);
	return (NULL);
}

char	**ft_split(char *str, char sep)
{
	int		i;
	int		words;
	char	**result;

	words = count_words(str, sep);
	result = (char **)malloc(sizeof(char *) * (words + 1));
	if (!result || !str)
		return (NULL);
	i = 0;
	while (*str)
	{
		if (!is_separator(*str, sep))
		{
			result[i] = extract_word(str, sep);
			if (!result[i])
				return (ft_freeall(result, (i - 1)));
			i++;
			while (*str && !is_separator(*str, sep))
				str++;
		}
		else
			str++;
	}
	result[i] = NULL;
	return (result);
}

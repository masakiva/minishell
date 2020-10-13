/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/09 19:25:31 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/02/11 15:05:52 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t		count_strings(const char *s, char c)
{
	size_t	nb_strings;

	nb_strings = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			while (*s != '\0' && *s != c)
				s++;
			nb_strings++;
			continue ;
		}
		s++;
	}
	return (nb_strings);
}

static const char	*dup_until_c(char **dest, const char *src, char c)
{
	size_t	len;
	size_t	i;

	while (*src == c)
		src++;
	len = 0;
	while (src[len] != '\0' && src[len] != c)
		len++;
	*dest = (char *)malloc(sizeof(char) * (len + 1));
	if (*dest == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		(*dest)[i] = src[i];
		i++;
	}
	(*dest)[i] = '\0';
	src += len + 1;
	return (src);
}

static void			free_strs(char ***strs, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		free((*strs)[i]);
		i++;
	}
	free(*strs);
	*strs = NULL;
}

char				**ft_split(const char *s, char c)
{
	char	**strs;
	size_t	nb_strings;
	size_t	i;

	nb_strings = count_strings(s, c);
	strs = (char **)malloc(sizeof(char *) * (nb_strings + 1));
	if (strs != NULL)
	{
		strs[nb_strings] = NULL;
		i = 0;
		while (i < nb_strings)
		{
			s = dup_until_c(strs + i, s, c);
			if (s == NULL)
			{
				free_strs(&strs, i);
				break ;
			}
			i++;
		}
	}
	return (strs);
}

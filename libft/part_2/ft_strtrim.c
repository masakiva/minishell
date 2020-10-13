/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 16:40:30 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/02/01 18:26:04 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static uint8_t	isinset(char const c, char const *set)
{
	while (*set != '\0')
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static size_t	trim_start(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0')
	{
		if (isinset(s1[i], set) == 0)
			break ;
		i++;
	}
	return (i);
}

static size_t	trim_end(char const *s1, char const *set, size_t i)
{
	while (i > 0)
	{
		i--;
		if (isinset(s1[i], set) == 0)
			break ;
	}
	return (i + 1);
}

char			*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	size_t	len;
	size_t	i;
	size_t	j;

	len = ft_strlen(s1);
	i = trim_start(s1, set);
	if (i != len)
		len = trim_end(s1, set, len);
	res = (char *)malloc(sizeof(char) * ((len - i) + 1));
	if (res != NULL)
	{
		j = 0;
		while (i < len)
		{
			res[j] = s1[i];
			j++;
			i++;
		}
		res[j] = '\0';
	}
	return (res);
}

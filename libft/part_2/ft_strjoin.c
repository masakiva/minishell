/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:44:06 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/30 12:07:04 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	size_t	i;

	join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	i = 0;
	if (join != NULL)
	{
		while (*s1 != '\0')
		{
			join[i] = *s1;
			i++;
			s1++;
		}
		while (*s2 != '\0')
		{
			join[i] = *s2;
			i++;
			s2++;
		}
		join[i] = '\0';
	}
	return (join);
}

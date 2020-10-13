/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 14:18:43 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/29 16:37:16 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char		*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	k;

	if (needle[0] == '\0' || (len > 0 && haystack == needle))
		return ((char *)haystack);
	i = 0;
	while (i < len && haystack[i] != '\0')
	{
		if (haystack[i] == needle[0])
		{
			j = i + 1;
			k = 1;
			while (j < len && haystack[j] == needle[k])
			{
				k++;
				if (needle[k] == '\0')
					return ((char *)(haystack + i));
				j++;
			}
		}
		i++;
	}
	return (NULL);
}

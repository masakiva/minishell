/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 18:42:21 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/29 16:25:04 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = 0;
	while (i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
		{
			ret = (int)(((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
			break ;
		}
		i++;
	}
	return (ret);
}

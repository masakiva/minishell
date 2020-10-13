/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:42:53 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/29 16:21:15 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	void	*ret;

	i = 0;
	ret = NULL;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
		{
			ret = (void *)(s + i);
			break ;
		}
		i++;
	}
	return (ret);
}

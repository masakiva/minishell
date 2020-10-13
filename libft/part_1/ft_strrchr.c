/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 21:30:37 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/29 16:27:40 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*ret;

	i = ft_strlen(s);
	if (c == '\0')
		ret = (char *)(s + i);
	else
	{
		ret = NULL;
		while (i > 0)
		{
			i--;
			if (s[i] == (char)c)
			{
				ret = (char *)(s + i);
				break ;
			}
		}
	}
	return (ret);
}

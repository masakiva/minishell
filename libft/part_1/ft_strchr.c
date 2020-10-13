/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 20:59:00 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/29 16:25:54 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			ret = (char *)(s + i);
			break ;
		}
		i++;
	}
	if (c == '\0')
		ret = (char *)(s + i);
	return (ret);
}

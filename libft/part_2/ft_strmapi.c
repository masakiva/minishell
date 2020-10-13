/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 15:19:08 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/30 12:46:45 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*new;
	size_t	i;

	new = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (new != NULL)
	{
		i = 0;
		while (s[i])
		{
			new[i] = (*f)(i, s[i]);
			i++;
		}
		new[i] = 0;
	}
	return (new);
}

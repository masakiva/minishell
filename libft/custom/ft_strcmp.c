/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:38:48 by abenoit           #+#    #+#             */
/*   Updated: 2020/12/26 00:02:16 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	//if (s1 == NULL && s2 == NULL)
		//return (0);
	//else if (s1 == NULL)
		//return (-((unsigned char*)s2)[i]);
	//else if (s2 == NULL)
		//return (((unsigned char*)s1)[i]);
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != !'\0')
	{
		i++;
	}
	return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
}

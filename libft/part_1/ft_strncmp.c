/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 12:06:44 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/29 16:27:17 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
		{
			ret = (int)((unsigned char)s1[i] - (unsigned char)s2[i]);
			break ;
		}
		i++;
	}
	return (ret);
}

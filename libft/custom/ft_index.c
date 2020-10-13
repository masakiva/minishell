/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_index.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 16:29:52 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/04 16:30:05 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

ssize_t	ft_index(char *haystack, char needle)
{
	ssize_t	i;

	i = 0;
	while (haystack[i] != '\0' && haystack[i] != needle)
		i++;
	if (haystack[i] != needle)
		i = -1;
	return (i);
}

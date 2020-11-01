/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:40:47 by abenoit           #+#    #+#             */
/*   Updated: 2020/11/01 22:41:30 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t		ft_isset(char c, char *set) //weird return value because it might be use to obtain char index
{
	size_t		i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (i);
		i++;
	}
	return (-1);
}

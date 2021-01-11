/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printarray_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:46:48 by abenoit           #+#    #+#             */
/*   Updated: 2021/01/12 00:05:04 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printarray_fd(char **array, int fd)
{
	size_t	i;

	i = 0;
	if (array != NULL)
		while (array[i] != NULL)
		{
			ft_putstr_fd(array[i], fd);
			ft_putchar_fd('\n', fd);
			i++;
		}
}

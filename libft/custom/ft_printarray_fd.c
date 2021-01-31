/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printarray_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:46:48 by abenoit           #+#    #+#             */
/*   Updated: 2021/01/31 13:20:46 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_printarray_fd(char **array, int fd)
{
	size_t	i;

	i = 0;
	if (array != NULL)
		while (array[i] != NULL)
		{
			if (ft_putstr_fd(array[i], fd) != WRITE_SUCCESS)
				return (FAILURE);
			if (ft_putchar_fd('\n', fd) != SUCCESS)
				return (FAILURE);
			i++;
		}
	return (SUCCESS);
}

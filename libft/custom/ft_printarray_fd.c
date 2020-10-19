/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printarray_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:46:48 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/19 13:47:00 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printarray_fd(char **array, int fd)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		ft_putstr_fd(array[i], fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
}

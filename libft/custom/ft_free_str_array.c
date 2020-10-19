/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_strarray.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:48:59 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/19 13:52:08 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	free_str_array(char ***ptr)
{
	int		i;

	i = 0;
	if (ptr != NULL)
	{
		if ((*ptr) != NULL)
		{
			while ((*ptr)[i] != NULL)
			{
				free((*ptr)[i]);
				(*ptr)[i] = NULL;
				i++;
			}
			free((*ptr));
			*ptr = NULL;
		}
	}
}

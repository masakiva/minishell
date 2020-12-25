/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_str_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 00:07:37 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/12/26 00:08:16 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	sort_str_array(char **array)
{
	size_t		i;
	size_t		j;

	i = 0;
	while (array[i] != NULL)
	{
		j = 0;
		while (array[j] != NULL)
		{
			if (ft_strcmp(array[i], array[j]) < 0)
				ft_swap((void **)&array[i], (void **)&array[j]);
			j++;
		}
		i++;
	}
}

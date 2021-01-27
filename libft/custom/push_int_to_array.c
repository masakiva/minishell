/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_int_to_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:53:41 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/01/27 11:53:57 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int		*push_int_to_array(int *array, size_t array_size, int nb)
{
	int		*new_array;
	size_t	i;

	new_array = (int *)malloc(sizeof(int) * (array_size + 1));
	if (new_array == NULL)
		return (NULL);
	i = 0;
	while (i < array_size)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = nb;
	free(array);
	return (new_array);
}

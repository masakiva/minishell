/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_str_from_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 02:13:47 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/12/26 22:52:27 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	**rm_str_from_array(char **array, size_t str_pos)
{
	size_t	array_size;
	char	**new_array;
	size_t	i;
	size_t	j;

	array_size = ft_arraylen(array);
	new_array = (char **)malloc(sizeof(char *) * array_size);
	if (new_array == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (i < array_size)
	{
		if (i != str_pos)
		{
			new_array[j] = array[i];
			j++;
		}
		else
			free(array[i]);
		i++;
	}
	new_array[j] = NULL;
	free(array);
	return (new_array);
}

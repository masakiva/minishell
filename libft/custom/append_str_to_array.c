/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_str_to_array.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 12:48:07 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/12/22 12:48:58 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	**append_str_to_array(char *str, char **array)
{
	size_t	array_size;
	char	**new_array;
	size_t	i;

	array_size = ft_arraylen(array);
	new_array = (char **)malloc(sizeof(char *) * (array_size + 2));
	if (new_array == NULL)
		return (NULL);
	i = 0;
	while (i < array_size)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = ft_strdup(str);
	new_array[i + 1] = NULL;
	return (new_array);
}

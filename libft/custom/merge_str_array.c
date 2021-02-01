/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_str_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 10:56:12 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/01 11:34:42 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>

char	**merge_str_array(char **ar1, char **ar2)
{
	size_t	array_size;
	char	**new_array;
	int		i;
	int		j;

	array_size = ft_arraylen(ar1) + ft_arraylen(ar2);
	new_array = (char **)malloc(sizeof(char *) * (array_size + 1));
	if (new_array == NULL)
		return (NULL);
	i = 0;
	while (ar1[i] != NULL)
	{
		new_array[i] = ar1[i];
		i++;
	}
	j = 0;
	while (ar2[j] != NULL)
	{
		new_array[i + j] = ar2[j];
		j++;
	}
	new_array[i + j] = NULL;
	free(ar1);
	free(ar2);
	return (new_array);
}

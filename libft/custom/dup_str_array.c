/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_str_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 15:19:15 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/12/16 15:42:42 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	**dup_str_array(char **array)
{
	char		**dup;
	size_t		i;
	size_t		arraylen;

	arraylen = ft_arraylen(array);
	dup = (char **)malloc(sizeof(char *) * (arraylen + 1));
	if (dup == NULL)
		return (NULL);
	ft_bzero(dup, sizeof(char *) * (arraylen + 1));
	i = 0;
	while (array[i] != NULL)
	{
		dup[i] = ft_strdup(array[i]);
		if (dup[i] == NULL)
		{
			free_str_array(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

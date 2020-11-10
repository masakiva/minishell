/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_var_content.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:42:49 by abenoit           #+#    #+#             */
/*   Updated: 2020/11/07 17:37:42 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*get_var_content(char **array, char *name)
{
	ssize_t		i;
	char	**tmp;
	char		*value;

	i = 0;
	while (array[i])
	{
		tmp = ft_split(array[i], '=');
		if (ft_strcmp(tmp[0], name) == 0)
		{
			free(tmp[0]);
			value = tmp[1];
			free(tmp);
			return (value);
		}
		free_str_array(&tmp);
		i++;
	}
	return (NULL);
}

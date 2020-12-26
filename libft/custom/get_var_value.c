/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:42:49 by abenoit           #+#    #+#             */
/*   Updated: 2020/12/20 16:52:19 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_var_value(char **env, char *name)
{
	size_t		name_len;

	while (*env != NULL)
	{
		name_len = ft_strlen(name);
		if ((size_t)ft_index(*env, '=') == name_len
			   && ft_strncmp(*env, name, name_len) == 0)
			return (ft_strdup(*env + name_len + 1));
		env++;
	}
	return (ft_strdup(""));
}
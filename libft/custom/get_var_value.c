/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:42:49 by abenoit           #+#    #+#             */
/*   Updated: 2020/12/16 15:27:34 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_var_value(char **env, char *name)
{
	while (*env != NULL)
	{
		if (ft_strncmp(*env, name, ft_strlen(name)) == 0)
			return (ft_strdup(ft_strchr(*env, '=') + 1));
		env++;
	}
	return (ft_strdup(""));
}

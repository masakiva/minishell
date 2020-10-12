/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:15:19 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/12 18:22:32 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#define ISNUM "0123456789"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(const char *str)
{
	while (*str)
	{
		ft_putchar(*str);
		str++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	if (s1 == NULL && s2 == NULL)
		return (0);
	else if (s1 == NULL)
		return (-((unsigned char*)s2)[i]);
	else if (s2 == NULL)
		return (((unsigned char*)s1)[i]);
	while (s1[i] == s2[i] && s1[i] && s2[i])
	{
		i++;
	}
	return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
}

int		ft_isset(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (i);
		i++;
	}
	return (-1);
}

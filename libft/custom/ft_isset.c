/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:40:47 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/19 13:42:04 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_isset(char c, char *set) //weird return value because it might be use to obtain char index
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 18:19:05 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/30 12:27:36 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*room;

	room = (void *)malloc(size * count);
	if (room != NULL)
		ft_bzero(room, size * count);
	return (room);
}

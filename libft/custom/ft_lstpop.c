/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 17:39:39 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/22 11:30:38 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_lstpop(t_list **lst)
{
	void	*ret;
	t_list	*ptr;

	if (lst == NULL)
		return (NULL);
	if (*lst == NULL)
		return (NULL);
	ret = (*lst)->content;
	ptr = (*lst)->next;
	free(*lst);
	*lst = NULL;
	*lst = ptr;
	return (ret);
}


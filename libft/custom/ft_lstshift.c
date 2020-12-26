/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstshift.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 17:39:39 by abenoit           #+#    #+#             */
/*   Updated: 2020/12/26 22:48:57 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_lstshift(t_list **lst)
{
	void	*ret;
	t_list	*tmp;

	if (lst == NULL || *lst == NULL)
		return (NULL);
	ret = (*lst)->content;
	tmp = (*lst)->next;
	free(*lst);
	*lst = tmp;
	return (ret);
}

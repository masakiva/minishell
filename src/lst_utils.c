/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 12:01:34 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 16:50:11 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_list	*ft_lstnew(void *content)
{
	t_list *new;

	if (!(new = malloc(sizeof(t_list))))
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_front(t_list **alst, t_list *new)
{
	if (new == NULL || alst == NULL)
		return ;
	new->next = *alst;
	*alst = new;
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *first;

	if (new == NULL || alst == NULL)
		return ;
	if (*alst == NULL)
		*alst = new;
	else
	{
		first = *alst;
		while (first->next != NULL)
			first = first->next;
		first->next = new;
		new->next = NULL;
	}
}

int		ft_lstsize(t_list *lst)
{
	int i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	ft_lstprint(void *content)
{
	char	*str;

	str = ((char*)content);
	ft_putstr(str);
	ft_putstr("\n");
}


void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (f == 0)
		return ;
	while (lst != NULL)
	{
		f(lst->content);
		lst = lst->next;
	}
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list *ptr;

	if (lst == NULL || del == NULL)
		return ;
	while (*lst != NULL)
	{
		ptr = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = ptr;
	}
	*lst = NULL;
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (del == NULL || lst == NULL)
		return ;
	del(lst->content);
	free(lst);
}

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
	*lst = ptr;
	return (ret);
}

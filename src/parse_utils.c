/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 22:36:58 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/10/15 12:43:26 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	print_token(void *token)
{
	static size_t	i = 0;

	printf("%zu = %s\n", i, (char *)token);
	i++;
}

int		link_token(t_list **tokens, t_state_machine *machine)
{
	t_list	*link; // "maillon"

	link = ft_lstnew(machine->cur_token);
	if (link != NULL)
		ft_lstadd_back(tokens, link);
	else
		return (FAILURE);
	machine->cur_token = NULL;
	return (SUCCESS);
}

int		reset_buf(t_state_machine *machine) // ou return res
{
	char	*res;
	
	if (machine->cur_token == NULL)
		res = ft_strdup(machine->buf); // on a la len de buf
	else
	{
		res = ft_strjoin(machine->cur_token, machine->buf); // idem
		free(machine->cur_token);
	}
	if (res == NULL)
		return (FAILURE);
	machine->cur_token = res;
	if (machine->len > 0)
	{
		machine->len = 0;
		ft_bzero(&machine->buf, BUF_SIZE);
	}
	return (SUCCESS);
}

int		add_to_buf(char c, t_state_machine *machine)
{
	if (machine->len == BUF_SIZE) // ou BUF_SIZE - 1
	{
		if (reset_buf(machine) == FAILURE)
			return (FAILURE);
	}
	machine->buf[machine->len] = c;
	machine->len++;
	return (SUCCESS);
}

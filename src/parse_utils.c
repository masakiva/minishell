#include "minishell.h"
#include <stdlib.h>

void	print_token(void *token)
{
	size_t	i = 0;

	printf("%zu = %s\n", i, (char *)token);
	i++;
}

int		link_token(t_list **tokens, t_state_machine *machine)
{
	t_list	*link; // "maillon"

	if (reset_buf(machine) == ERROR)
		return (ERROR); // malloc error
	link = ft_lstnew(machine->cur_token);
	if (link != NULL)
		ft_lstadd_back(tokens, link);
	else
		return (ERROR);
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
		return (ERROR);
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
		if (reset_buf(machine) == ERROR)
			return (ERROR);
	}
	machine->buf[machine->len] = c;
	machine->len++;
	return (SUCCESS);
}

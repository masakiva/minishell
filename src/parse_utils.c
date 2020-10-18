#include "minishell.h"
#include <stdlib.h>

void	print_tokens(t_list *tokens)
{
	size_t		i;
	t_token		*token;
	t_list		*vars;
	t_variable	*var;

	i = 0;
	while (tokens != NULL)
	{
		token = (t_token *)tokens->content;
		printf("%zu = %s\n", i, token->str);
		if (token->vars != NULL)
		{
			vars = token->vars;
			while (vars != NULL)
			{
				var = vars->content;
				printf("var starts at %zu ('%c') and ends at %zu ('%c')\n",
						var->start, token->str[var->start],
						var->end, token->str[var->end]);
				vars = vars->next;
			}
		}
		tokens = tokens->next;
		i++;
	}
}

int		link_token(t_list **tokens, t_state_machine *machine)
{
	t_list	*link; // "maillon"

	if (reset_buf(machine) == ERROR) // laisser en dehors de la func?
		return (ERROR);
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
	{
		machine->cur_token = (t_token *)malloc(sizeof(t_token));
		if (machine->cur_token == NULL)
			return (ERROR);
		machine->cur_token->vars = NULL;
		res = ft_strdup(machine->buf); // on a la len de buf
	}
	else
	{
		res = ft_strjoin(machine->cur_token->str, machine->buf); // idem
		free(machine->cur_token->str);
	}
	if (res == NULL)
		return (ERROR);
	machine->cur_token->str = res;
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

#include "parsing.h"

int		add_arg(t_state_machine *machine)
{
	if (reset_buf(machine) == FAILURE)
		return (FAILURE);
	if (machine->cur_token_stack == &machine->redir_paths
			&& ((machine->var_token_count >= 1 && machine->cur_arg[0] != '\0')
				|| (machine->var_state == TRUE && machine->var_token_count == 0
					&& machine->cur_arg[0] == '\0')))
	{
		machine->redir_types[0] = AMBIG;
	}
	else if (machine->quote_state == TRUE || machine->cur_arg[0] != '\0')
	{
		*machine->cur_token_stack = push_str_to_array(*machine->cur_token_stack,
				machine->cur_arg);
		if (*machine->cur_token_stack == NULL)
			return (FAILURE);
		machine->cur_arg = NULL;
	}
	machine->quote_state = FALSE;
	return (SUCCESS);
}

int		reset_buf(t_state_machine *machine)
{
	if (machine->cur_arg == NULL)
		machine->cur_arg = ft_strdup(machine->buf);
	else if (machine->len != 0)
		machine->cur_arg = strjoin_free(machine->cur_arg, machine->buf);
	if (machine->cur_arg == NULL)
		return (FAILURE);
	if (machine->len != 0)
	{
		machine->len = 0;
		ft_bzero(&machine->buf, BUF_SIZE);
	}
	return (SUCCESS);
}

int		add_to_buf(t_state_machine *machine, char c)
{
	if (machine->len == BUF_SIZE - 1)
	{
		if (reset_buf(machine) == FAILURE)
			return (FAILURE);
	}
	machine->buf[machine->len] = c;
	machine->len++;
	return (SUCCESS);
}

void	free_command(void *content)
{
	t_command	*command;

	command = (t_command *)content;
	free_str_array(command->args);
	free_str_array(command->redir_paths);
	free(command->redir_types);
	free(command);
}

void	free_commands(t_list **commands)
{
	ft_lstclear(commands, free_command);
}

#include "parsing.h"

int		parse_exit_status(t_state_machine *machine)
{
	char	*value;
	size_t	i;

	value = ft_itoa(machine->stat_loc);
	if (value == NULL)
		return (FAILURE);
	i = 0;
	while (value[i] != '\0')
	{
		add_to_buf(machine, value[i]);
		i++;
	}
	free(value);
	return (SUCCESS);
}

char	*parse_quoted_variable(t_state_machine *machine, char *line)
{
	size_t	var_name_len;
	char	*var_value;
	size_t	i;

	var_name_len = ft_varnamelen(line);
	var_value = get_var_value(machine->env, line, var_name_len);
	if (var_value == NULL)
		return (NULL);
	i = 0;
	while (var_value[i] != '\0')
	{
		add_to_buf(machine, var_value[i]);
		i++;
	}
	free(var_value);
	line += var_name_len;
	return (line);
}

int			blank_str(char *line) // duplicate of empty_command
{
	while (ft_isspace(*line))
		line++;
	if (*line == '\0')
		return (TRUE);
	return (FALSE);
}

#include <stdio.h>
char	*parse_variable(t_state_machine *machine, char *line)
{
	size_t	var_name_len;
	char	*var_value;
	size_t	i;

	if (reset_buf(machine) == FAILURE)
		return (NULL);
	var_name_len = ft_varnamelen(line);
	var_value = get_var_value(machine->env, line, var_name_len);
	if (var_value == NULL)
		return (NULL);
	i = 0;
	if (ft_isspace(var_value[i]) && machine->cur_arg[0] != '\0')
	{
		if (add_arg(machine) == FAILURE)
		{
			free(var_value);
			return (NULL);
		}
		i++;
	}
	while (ft_isspace(var_value[i]))
		i++;
	while (var_value[i] != '\0')
	{
		if (ft_isspace(var_value[i]))
		{
			if (add_arg(machine) == FAILURE)
			{
				free(var_value);
				return (NULL);
			}
			i++;
			if (blank_str(var_value + i) == TRUE)
				break ;
			while (ft_isspace(var_value[i]))
				i++;
		}
		add_to_buf(machine, var_value[i]);
		i++;
	}
	free(var_value);
	line += var_name_len;
	return (line);
}

int		add_arg(t_state_machine *machine)
{
	if (reset_buf(machine) == FAILURE)
		return (FAILURE);
	machine->args = push_str_to_array(machine->args, machine->cur_arg);
	if (machine->args == NULL)
	{
		free(machine->cur_arg);
		return (FAILURE);
	}
	machine->cur_arg = NULL;
	return (SUCCESS);
}

int		reset_buf(t_state_machine *machine)
{
	if (machine->cur_arg == NULL)
		machine->cur_arg = ft_strdup(machine->buf); // on a la len de buf
	else if (machine->len != 0)
		machine->cur_arg = strjoin_free(machine->cur_arg, machine->buf); // idem
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_vars_redirs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 12:14:56 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/01 11:24:26 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "libft.h"

static int	add_var_args(t_state_machine *machine, char *var_value)
{
	if (ft_isspace(*var_value) && machine->cur_arg[0] != '\0')
	{
		if (add_arg(machine) == FAILURE)
			return (FAILURE);
		machine->var_token_count++;
		var_value++;
	}
	while (ft_isspace(*var_value))
		var_value++;
	while (*var_value != '\0')
	{
		if (ft_isspace(*var_value))
		{
			if (add_arg(machine) == FAILURE)
				return (FAILURE);
			machine->var_token_count++;
			if (blank_str(var_value) == TRUE)
				break ;
			while (ft_isspace(*var_value))
				var_value++;
		}
		if (add_to_buf(machine, *var_value++) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

char		*parse_variable(t_state_machine *machine, char *line)
{
	size_t	var_name_len;
	char	*var_value;

	if (reset_buf(machine) == FAILURE)
		return (NULL);
	var_name_len = ft_varnamelen(line);
	var_value = get_var_value(machine->env, line, var_name_len);
	if (var_value == NULL)
		return (NULL);
	if (add_var_args(machine, var_value) == FAILURE)
	{
		free(var_value);
		return (NULL);
	}
	free(var_value);
	line += var_name_len;
	machine->var_state = TRUE;
	return (line);
}

int			parse_exit_status(t_state_machine *machine)
{
	char	*value;
	size_t	i;

	value = ft_itoa(machine->stat_loc);
	if (value == NULL)
		return (FAILURE);
	i = 0;
	while (value[i] != '\0')
	{
		if (add_to_buf(machine, value[i]) == FAILURE)
		{
			free(value);
			return (FAILURE);
		}
		i++;
	}
	free(value);
	return (SUCCESS);
}

char		*parse_quoted_variable(t_state_machine *machine, char *line)
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
		if (add_to_buf(machine, var_value[i]) == FAILURE)
		{
			free(var_value);
			return (NULL);
		}
		i++;
	}
	free(var_value);
	line += var_name_len;
	return (line);
}

char		*new_redir_info(t_state_machine *machine, char *line)
{
	enum e_redir_op	redir_type;

	if (*line == '<')
		redir_type = FILEIN;
	else if (line[1] == '>')
	{
		redir_type = APPEND;
		line++;
	}
	else
		redir_type = FILEOUT;
	machine->redir_types =
		(enum e_redir_op *)push_int_to_array((int *)machine->redir_types,
				ft_arraylen(machine->redir_paths), (int)redir_type);
	return (line);
}

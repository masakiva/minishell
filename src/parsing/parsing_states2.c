#include "parsing.h"

char	*single_quote(t_state_machine *machine, char *line)
{
	while (*line != '\'')
	{
		if (add_to_buf(machine, *line) == FAILURE)
			return (NULL);
		line++;
	}
	line++;
	machine->state = LETTER;
	machine->quote_state = TRUE;
	return (line);
}

char	*double_quote(t_state_machine *machine, char *line)
{
	if (*line == '"')
	{
		machine->state = LETTER;
		machine->quote_state = TRUE;
	}
	else if (*line == '$')
		machine->state = QUOTED_DOLLAR;
	else if (*line == '\\')
		machine->state = QUOTED_BACKSLASH;
	else if (add_to_buf(machine, *line) == FAILURE)
		return (NULL);
	line++;
	return (line);
}

char	*quoted_backslash(t_state_machine *machine, char *line)
{
	if (ft_isset(*line, "\\\"$"))
	{
		if (add_to_buf(machine, *line) == FAILURE)
			return (NULL);
		line++;
	}
	else
	{
		if (add_to_buf(machine, '\\') == FAILURE)
			return (NULL);
	}
	machine->state = DOUBLE_QUOTE;
	return (line);
}

char	*quoted_dollar(t_state_machine *machine, char *line)
{
	if (*line == '?')
	{
		if (parse_exit_status(machine) == FAILURE)
			return (NULL);
		line++;
	}
	else if (ft_isalnum(*line) || *line == '_')
	{
		line = parse_quoted_variable(machine, line);
		if (line == NULL)
			return (NULL);
	}
	else if (add_to_buf(machine, '$') == FAILURE)
		return (NULL);
	machine->state = DOUBLE_QUOTE;
	return (line);
}

char	*angle_bracket(t_state_machine *machine, char *line)
{
	line = new_redir_info(machine, line);
	if (line == NULL)
		return (NULL);
	line++;
	machine->state = SPACE;
	machine->cur_token_stack = &machine->redir_paths;
	return (line);
}

#include "parsing.h"

char	*letter(t_state_machine *machine, char *line)
{
	if (ft_isspace(*line) || ft_isset(*line, "><;|") || *line == '\0')
	{
		if (add_arg(machine) == FAILURE)
			return (NULL);
		machine->cur_token_stack = &machine->args;
		machine->var_token_count = 0;
		machine->var_state = FALSE;
		machine->state = SPACE;
	}
	else
	{
		if (*line == '\\')
			machine->state = BACKSLASH;
		else if (*line == '$')
			machine->state = DOLLAR;
		else if (*line == '\'')
			machine->state = SINGLE_QUOTE;
		else if (*line == '"')
			machine->state = DOUBLE_QUOTE;
		else if (add_to_buf(machine, *line) == FAILURE)
			return (NULL);
		line++;
	}
	return (line);
}

char	*space(t_state_machine *machine, char *line)
{
	if (ft_isspace(*line))
		line++;
	else if (*line == '>' || *line == '<')
		machine->state = ANGLE_BRACKET;
	else if (*line == '|')
	{
		machine->state = END;
		machine->pipe_flag = TRUE;
		line++;
	}
	else if (*line == ';')
	{
		machine->state = END;
		line++;
	}
	else if (*line == '\0')
		machine->state = END;
	else
		machine->state = LETTER;
	return (line);
}

char	*backslash(t_state_machine *machine, char *line)
{
	if (add_to_buf(machine, *line) == FAILURE)
		return (NULL);
	line++;
	machine->state = LETTER;
	return (line);
}

char	*dollar(t_state_machine *machine, char *line)
{
	if (*line == '"' || *line == '\'')
		machine->state = LETTER;
	else if (*line == '?')
	{
		if (parse_exit_status(machine) == FAILURE)
			return (NULL);
		line++;
		machine->state = LETTER;
	}
	else if (ft_isalnum(*line) || *line == '_')
	{
		line = parse_variable(machine, line);
		if (line == NULL)
			return (NULL);
		machine->state = LETTER;
	}
	else
	{
		if (add_to_buf(machine, '$') == FAILURE)
			return (NULL);
		machine->state = LETTER;
	}
	return (line);
}

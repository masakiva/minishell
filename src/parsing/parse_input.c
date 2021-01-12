#include "parsing.h"

char	*angle_bracket(t_state_machine *machine, char *line)
{
//	if (reset_buf(machine) == FAILURE)
//		return(NULL);
//	if (machine->cur_token->redir != NO_REDIR)
//		machine->error = REDIR_PATH_INVALID;
//	else
//	{
	line = new_redir_info(machine, line);
	if (line == NULL)
		return (NULL);
	line++;
	machine->state = SPACE;
	machine->cur_token_stack = &machine->redir_paths;
//	}
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
	else
		add_to_buf(machine, '$');
	machine->state = DOUBLE_QUOTE;
	return (line);
}

char	*quoted_backslash(t_state_machine *machine, char *line)
{
	if (!ft_isset(*line, "\\\"$"))
		add_to_buf(machine, '\\');
	machine->state = DOUBLE_QUOTE;
	return (line);
}

char	*double_quote(t_state_machine *machine, char *line)
{
	if (*line == '"')
		machine->state = LETTER;
	else if (*line == '$')
		machine->state = QUOTED_DOLLAR;
	else if (*line == '\\')
		machine->state = QUOTED_BACKSLASH;
	else
		add_to_buf(machine, *line);
	line++;
	return (line);
}

char	*single_quote(t_state_machine *machine, char *line)
{
	while (*line != '\'')
	{
		add_to_buf(machine, *line);
		line++;
	}
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
		machine->state = LETTER; // don't finish arg when empty var value
	}
	else
	{
		add_to_buf(machine, '$');
		machine->state = LETTER;
	}
	return (line);
}

char	*backslash(t_state_machine *machine, char *line)
{
	add_to_buf(machine, *line);
	line++;
	machine->state = LETTER;
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

char	*letter(t_state_machine *machine, char *line)
{
	if (ft_isspace(*line) || ft_isset(*line, "><;|") || *line == '\0')
	{
		if (add_arg(machine) == FAILURE)
			return (NULL);
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
		else
			add_to_buf(machine, *line);
		line++;
	}
	return (line);
}

//#include <stdio.h>
char	**parse_one_command(char **line, char **env, int stat_loc, t_command *command)
{
	static t_parse	process[NB_STATES - 1] = {space, letter, backslash, dollar,
		single_quote, double_quote, quoted_backslash, quoted_dollar, angle_bracket};
	t_state_machine	machine;

//	if ((*line)[0] == '.' && (*line)[1] == '\0')
//		exit(EXIT_FAILURE);
	ft_bzero(&machine, sizeof(machine));
	machine.env = env;
	machine.stat_loc = stat_loc;
	machine.cur_token_stack = &machine.args;
	while (machine.state != END)
	{
		*line = process[machine.state](&machine, *line);
		if (*line == NULL)
			break ;
	}
	command->pipe_flag = machine.pipe_flag;
	command->redir_paths = machine.redir_paths;
	command->redir_types = machine.redir_types;
//	ft_printarray_fd(machine.args, STDOUT_FILENO);
//	printf("REDIR PATHS:\n");
//	ft_printarray_fd(machine.redir_paths, STDOUT_FILENO);
	return (machine.args);// could be saved directly in command
}

int			empty_command(char *line)
{
	while (ft_isspace(*line))
		line++;
	if (*line == '\0')
		return (TRUE);
	return (FALSE);
}

t_command	*parse_commands(t_xe *xe)
{
	t_command	*cur_command;

	cur_command = malloc(sizeof(t_command));
	if (cur_command == NULL)
		return (NULL);
	cur_command->pipe_flag = FALSE;
	if (empty_command(xe->line) == FALSE)
	{
		cur_command->args = parse_one_command(&(xe->line), xe->env, xe->stat_loc,
				cur_command);
		if (cur_command->args == NULL)
			return (NULL);
		return (cur_command);
	}
	else
	{
		cur_command->pipe_flag = LINE_END;
		return (cur_command);
	}
}

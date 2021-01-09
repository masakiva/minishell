#include "parsing.h"

//int		parsing_error(t_list **commands, t_state_machine *machine)
//{
//	static char		*err_msg[NB_PARSING_ERRORS] = {
//		"=== no error (here for padding) ===",
//		"No matching single quote",
//		"No matching double quote",
//		"Multiline inputs are currently not supported",
//		"Redirection path invalid",
//		"No redirection path specified",
//		"Empty command before ; or |"};
//
//	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
//	ft_putstr_fd(err_msg[machine->error], STDERR_FILENO);
//	ft_putchar_fd('\n', STDERR_FILENO);
//	if (machine->cur_token != NULL)
//	{
//		free_token(machine->cur_token);
//		free_commands(commands);
//	}
//	else
//		*commands = NULL;/////// no need to free?
//	return (PARSING_ERR);
//}
//
//char	*dollar(t_state_machine *machine, char *line)
//{
//	line++;
//	if (*line == '"' || *line == '\'')
//		machine->state = QUOTE;
//	else if (*line == '?' || ft_isalnum(*line) || *line == '_')
//	{
//		line = parse_variable(line, machine, TO_SPLIT);
//		if (line == NULL)
//			return (NULL);
//		machine->state = LETTER;
//	}
//	else
//	{
//		add_to_buf(machine, '$');
//		machine->state = LETTER;
//	}
//	return (line);
//}
//
//char	*quote(t_state_machine *machine, char *line)
//{
//	char	quote_style;
//
//	(void)commands;
//	quote_style = *line;
//	line++;
//	while (*line != quote_style && *line != '\0')
//	{
//		if (quote_style == '"' && *line == '$')
//		{
//			line++;
//			if (*line == '?' || ft_isalnum(*line) || *line == '_')
//			{
//				line = parse_variable(line, machine, NOT_TO_SPLIT);
//				if (line == NULL)
//					return (NULL);
//			}
//			else
//				add_to_buf(machine, '$');
//		}
//		else
//		{
//			if (quote_style == '"' && *line == '\\'
//					&& ft_isset(line[1], "\\\"$"))
//				line++;
//			add_to_buf(machine, *line);
//			line++;
//		}
//	}
//	if (*line == '\0')
//		machine->error = ft_index("'\"", quote_style);
//	else
//	{
//		line++;
//		machine->state = LETTER;
//	}
//	return (line);
//}
//
//char	*angle_bracket(t_state_machine *machine, char *line)
//{
//	(void)commands;
//	if (reset_buf(machine) == FAILURE)
//		return(NULL);
//	if (machine->cur_token->redir != NO_REDIR)
//		machine->error = REDIR_PATH_INVALID;
//	else
//	{
//		if (*line == '<')
//			machine->cur_token->redir = FILEIN;
//		else if (line[1] == '>')
//		{
//			machine->cur_token->redir = APPEND;
//			line++;
//		}
//		else
//			machine->cur_token->redir = FILEOUT;
//		line++;
//		machine->state = SPACE;
//	}
//	return (line);
//}
//
//char	*tilde(t_state_machine *machine, char *line)
//{
//	line++;
//	add_to_buf(machine, '~');
//	if (reset_buf(machine) == FAILURE)
//		return (NULL);
//	if (machine->cur_token->str[1] == '\0'
//			&& (ft_isset(*line, "/><;|") || ft_isspace(*line) || *line == '\0'))
//	{
//		if (add_variable(&machine->cur_token->var_properties, 0, 1, TO_SPLIT)
//				== FAILURE)
//		{
//			free_token(machine->cur_token);
//			return (NULL);
//		}
//	}
//	machine->state = LETTER;
//	return (line);
//}

char	*backslash(t_state_machine *machine, char *line)
{
	line++;
	add_to_buf(machine, *line);
	line++;
	machine->state = LETTER;
	return (line);
}

char	*space(t_state_machine *machine, char *line)
{
	if (ft_isspace(*line))
		line++;
//	else if (*line == '>' || *line == '<')
//		machine->state = ANGLE_BRACKET;
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
	if (*line == '\\')
		machine->state = BACKSLASH;
//	else if (*line == '"' || *line == '\'')
//		machine->state = QUOTE;
//	else if (*line == '$')
//		machine->state = DOLLAR;
//	else if (*line == '~')
//		machine->state = TILDE;
	else if (ft_isspace(*line) || ft_isset(*line, ";|"/*"><;|"*/) || *line == '\0')
	{
		if (add_arg(machine) == FAILURE)
			return (NULL);
		machine->state = SPACE;
	}
	else
	{
		add_to_buf(machine, *line);
		line++;
	}
	return (line);
}

char	**parse_one_command(char **line, t_byte *pipe_flag)
{
	static t_parse	process[NB_STATES - 1] = {space, letter, backslash};
	t_state_machine	machine;
	char			*orig_line;

	orig_line = *line;
	ft_bzero(&machine, sizeof(machine));
	while (machine.state != END)
	{
		*line = process[machine.state](&machine, *line);
		if (*line == NULL)
			break ;
	}
	*pipe_flag = machine.pipe_flag;
	if (*line != NULL)
		*line = ft_strdup(*line);
	free(orig_line);
	if (*line == NULL)
	{
		free_str_array(machine.args);
		return (NULL);
	}
	ft_printarray_fd(machine.args, STDOUT_FILENO);
	return (machine.args);
}

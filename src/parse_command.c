#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

char	*error(char *line, t_list **commands, t_state_machine *machine)
{
	static char		*err_msg[NB_PARSING_ERRORS] = {
		"No maching single quote",
		"No maching double quote",
		"Multiline inputs are currently not supported", // mais en fait si je pense
		"Token after redirection operator invalid"};
	(void)commands;
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[machine->error], STDERR_FILENO);// ou all->fd[2]?
	ft_putchar_fd('\n', STDERR_FILENO);
//	if (machine->cur_token != NULL)
//	{
//		free(machine->cur_token->str);
//		free(machine->cur_token);
//	}
	machine->state = END;
	return (line);
}

char	*space(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (*line == '\0')
		machine->state = END;
	else if (ft_isspace(*line))
		line++;
	else if (*line == '>' || *line == '<')
		machine->state = ANGLE_BRACKET;
	else
		machine->state = LETTER;
	return (line);
}

char	*backslash(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (*line != '\0')
	{
		add_to_buf(*line, machine);
		line++;
		machine->state = LETTER;
	}
	else
	{
		machine->state = ERR;
		machine->error = ESCAPE_NL;
	}
	return (line);
}

char	*dollar(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '?' || ft_isalnum(*line))
	{
		line = parse_variable(line, machine);
		machine->state = LETTER;
	}
	else
	{
		add_to_buf('$', machine);
		machine->state = LETTER;
	}
	return (line);
}

char	*quote(char *line, t_list **commands, t_state_machine *machine)
{
	char	quote_style;

	(void)commands;
	quote_style = *line;
	line++;
	while (*line != quote_style && *line != '\0')
	{
		if (quote_style == '"' && *line == '$')
		{
			line++;
			if (*line == '?' || ft_isalnum(*line))
				line = parse_variable(line, machine);
			else
				add_to_buf('$', machine);
		}
		else
		{
			if (quote_style == '"' && *line == '\\'
					&& ft_isset(line[1], "\\\"$") != -1)
				line++;
			add_to_buf(*line, machine);
			line++;
		}
	}
	if (*line == '\0')
	{
		machine->state = ERR;
		machine->error = ft_index("'\"", quote_style);
	}
	else
	{
		line++;
		machine->state = LETTER;
	}
	return (line);
}

char	*angle_bracket(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (reset_buf(machine) == ERROR)
		err_bis(MALLOC_ERR);
	if (machine->cur_token->redir != NO_REDIR)
	{
		machine->state = ERR;
		machine->error = REDIR_TOKEN;
	}
	else
	{
		if (*line == '<')
			machine->cur_token->redir = FILEIN;
		else if (line[1] == '>')
		{
			machine->cur_token->redir = APPEND;
			line++;
		}
		else
			machine->cur_token->redir = FILEOUT;
		line++;
		machine->state = SPACE;
	}
	return (line);
}

char	*letter(char *line, t_list **commands, t_state_machine *machine)
{
	t_list		**tokens;

	tokens = (t_list **)(*commands)->content;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '>' || *line == '<')
	{
		if (link_token(tokens, machine) == ERROR)
			err_bis(MALLOC_ERR);
		machine->state = ANGLE_BRACKET;
	}
	else if (*line == '$')
	{
		machine->state = DOLLAR;
		line++;
	}
	else if (*line == '\\')
	{
		machine->state = BACKSLASH;
		line++;
	}
	else if (*line == '\0' || ft_isspace(*line))
	{
		if (link_token(tokens, machine) == ERROR)
			err_bis(MALLOC_ERR);
		if (*line == '\0')
			machine->state = END;
		else
		{
			machine->state = SPACE;
			line++;
		}
	}
	else
	{
		add_to_buf(*line, machine);
		line++;
	}
	return (line);
}

void	parse_input(char *line)
{
	static t_function	process[NB_STATES - 1] = {letter, quote, backslash,
		dollar, space, angle_bracket, error};
	t_state_machine		machine;
	t_list				*commands;
	t_list				*tokens;

	errno = 0;
	machine.state = SPACE;
	machine.len = 0;
	machine.cur_token = NULL;
	ft_bzero(&machine.buf, BUF_SIZE);
	tokens = NULL;
	commands = ft_lstnew(&tokens);
	if (line[0] == '.' && line[1] == '\0')
		exit(EXIT_SUCCESS);
	while (machine.state != END)
	{
		line = process[machine.state](line, &commands, &machine);
		//printf("current = %c ; state = %d\n", *line, machine.state);
	}
	if (machine.cur_token != NULL && machine.cur_token->redir != NO_REDIR)
	{
		printf("ERROR NO REDIR PATH\n");
		free(machine.cur_token->str);
		free(machine.cur_token);
	}
	printf("################# lstsize = %zu\n", ft_lstsize(tokens));
	print_tokens(tokens);
	ft_lstclear(&tokens, free_token);
	free(commands);
}

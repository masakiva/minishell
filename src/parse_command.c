#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

void		free_elem(void *content)
{
	free(content);
}

void		free_token(void *content)
{
	t_token		*token;

	token = (t_token *)content;
	free(token->str);
	ft_lstclear(&token->vars, free_elem);
	free(content);
}

size_t		error(char *line, t_list **tokens, t_state_machine *machine)
{
	static char		*err_msg[NB_PARSING_ERRORS] = {
		"No maching single quote",
		"No maching double quote",
		"Multiline inputs are currently not supported"}; // mais en fait si je pense
	(void)line;
	(void)tokens;
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[machine->error], STDERR_FILENO);// ou all->fd[2]?
	ft_putchar_fd('\n', STDERR_FILENO);
	machine->state = END;
	return (0);
}

size_t		space(char *line, t_list **tokens, t_state_machine *machine)
{
	size_t		elem_len;

	(void)tokens;
	elem_len = 0;
	if (*line == '\0')
		machine->state = END;
	else if (ft_isspace(*line))
		elem_len++;
	else
		machine->state = LETTER;
	return (elem_len);
}

size_t		backslash(char *line, t_list **tokens, t_state_machine *machine)
{
	(void)tokens;
	if (*line != '\0')
	{
		add_to_buf(*line, machine);
		machine->state = LETTER;
	}
	else
	{
		machine->state = ERR;
		machine->error = ESCAPE_NL;
	}
	return (1);
}

size_t		quote(char *line, t_list **tokens, t_state_machine *machine)
{
	char	quote_style;
	size_t	elem_len;

	(void)tokens;
	quote_style = *line;
	line++;
	elem_len = 0;
	while (*line != quote_style && *line != '\0')
	{
		if (quote_style == '"' && *line == '\\')
		{
			if(ft_isset(line[1], "\\\"$") != -1)
			{
				line++;
				elem_len++;
			}
		}
		else if (quote_style == '"' && *line == '$') //to be continued
			;
		add_to_buf(*line, machine);
		line++;
		elem_len++;
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
	return (elem_len + 2);
}

int		add_var_to_expand(t_list **variables, size_t start, size_t end)
{
	t_list		*link;
	t_variable	*variable;

	variable = (t_variable *)malloc(sizeof(t_variable));
	if (variable == NULL)
		return (ERROR);
	variable->start = start;
	variable->end = end;
	link = ft_lstnew(variable);
	if (link != NULL)
		ft_lstadd_back(variables, link);
	else
		return (ERROR);
	return (SUCCESS);
}

size_t		dollar(char *line, t_list **tokens, t_state_machine *machine)
{
	size_t		elem_len;
	size_t		var_end;

	(void)tokens;
	elem_len = 0;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '?' || ft_isdigit(*line)) // second condition to keep?
	{
		add_to_buf(*line, machine);
		elem_len++;
		if (reset_buf(machine) == ERROR)
			err_bis(MALLOC_ERR);
		var_end = ft_strlen(machine->cur_token->str) - 1;
		add_var_to_expand(&machine->cur_token->vars, var_end - elem_len + 1, var_end);
		machine->state = LETTER;
	}
	else if (ft_isalnum(*line) != 0)
	{
		while (ft_isalnum(*line))
		{
			add_to_buf(*line, machine);
			line++;
			elem_len++;
		}
		if (reset_buf(machine) == ERROR)
			err_bis(MALLOC_ERR);
		var_end = ft_strlen(machine->cur_token->str) - 1;
		add_var_to_expand(&machine->cur_token->vars, var_end - elem_len + 1, var_end);
		machine->state = LETTER;
	}
	else
	{
		add_to_buf('$', machine);
		machine->state = LETTER;
	}
	return (elem_len);
}

size_t		angle_bracket(char *line, t_list **tokens, t_state_machine *machine)
{
	size_t		elem_len;

	elem_len = 1;
	(void)line;
	(void)tokens;
	(void)machine;
	return (elem_len);
}

size_t		letter(char *line, t_list **tokens, t_state_machine *machine)
{
	size_t		elem_len;

	elem_len = 0;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '>' || *line == '<')
		machine->state = ANGLE_BRACKET;
	else if (*line == '$')
	{
		machine->state = DOLLAR;
		elem_len++;
	}
	else if (*line == '\\')
	{
		machine->state = BACKSLASH;
		elem_len++;
	}
	else if (*line == '\0' || ft_isspace(*line))
	{
		if (link_token(tokens, machine) == ERROR) //si la ligne est vide ou commence par un space on devra retirer le token
			err_bis(MALLOC_ERR);
		if (*line == '\0')
			machine->state = END;
		else
		{
			machine->state = SPACE;
			elem_len++;
		}
	}
	else
	{
		add_to_buf(*line, machine);
		elem_len++;
	}
	return (elem_len);
}

void	parse_input(char *line)
{
	static t_function	process[NB_STATES - 1] = {letter, quote, backslash,
		dollar, space, angle_bracket, error};
	t_state_machine		machine;
	size_t				elem_len;
	t_list				*tokens;

	errno = 0;
	machine.state = LETTER;
	machine.len = 0;
	machine.cur_token = NULL;
	ft_bzero(&machine.buf, BUF_SIZE);
	tokens = NULL;
	if (line[0] == '.' && line[1] == '\0')
		exit(EXIT_SUCCESS);
	while (machine.state != END)
	{
		elem_len = process[machine.state](line, &tokens, &machine);
		line += elem_len;
		//printf("current = %c ; state = %d\n", *line, machine.state);
	}
	printf("_______________lstsize = %zu\n", ft_lstsize(tokens));
	print_tokens(tokens);
	ft_lstclear(&tokens, free_token);
}

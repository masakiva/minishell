#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

size_t		error(char *line, t_list **tokens, t_state_machine *machine)
{
	static char		*err_msg[NB_PARSING_ERRORS] = {
		"No maching single quote",
		"No maching double quote"};
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
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '\0')
		machine->state = END;
	else if (ft_isset(*line, ISSPACE_3) == -1)
		machine->state = LETTER;
	else
		elem_len++;
	return (elem_len);
}

size_t		backslash(char *line, t_list **tokens, t_state_machine *machine)
{
	(void)tokens;
	if (ft_isset(*line, "'\"$|;<>\\") != -1)
		add_to_buf(*line, machine);
	else
		;
	return (1);
}

size_t		quote(char *line, t_list **tokens, t_state_machine *machine)
{
	char	quote_style;
	size_t	elem_len;

	quote_style = *line;
	line++;
	elem_len = 0;
	while (*line != quote_style && *line != '\0')
	{
		add_to_buf(*line, machine);
		line++;
		elem_len++;
	}
	if (*line != '\0')
	{
		line++;
		if (*line == '\0' || ft_isset(*line, ISSPACE_3) != -1)
		{
			if (link_token(tokens, machine) == ERROR)
				err_bis(MALLOC_ERR);
			if (*line == '\0')
				machine->state = END;
			else
			{
				machine->state = SPACE;
				elem_len++;
			}
		}
		else if (*line != '"' && *line != '\'')
			machine->state = LETTER;
	}
	else
	{
		machine->state = ERR;
		machine->error = ft_index("'\"", quote_style);
	}
	return (elem_len + 2);
}

size_t		letter(char *line, t_list **tokens, t_state_machine *machine)
{
	size_t		elem_len;

	elem_len = 0;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '\\')
	{
		machine->state = BACKSLASH;
		elem_len++;
	}
	else if (*line == '\0' || ft_isset(*line, ISSPACE_3) != -1)
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

char			**split_command(char *line)
{
	static t_function	process[NB_STATES - 1] = {letter, quote, backslash, space, error};
	t_state_machine		machine;
	size_t				elem_len;
	t_list				*tokens;

	errno = 0;
	machine.state = LETTER;
	machine.len = 0;
	machine.cur_token = NULL;
	ft_bzero(&machine.buf, BUF_SIZE);
	tokens = NULL;
	while (machine.state != END)
	{
		elem_len = process[machine.state](line, &tokens, &machine);
		line += elem_len;
		//printf("current = %c ; state = %d\n", *line, machine.state);
	}
	printf("lstsize = %zu\n", ft_lstsize(tokens));
	size_t i = 0;
	while (tokens != NULL)
	{
		printf("%zu = %s\n", i, (char *)(tokens->content));
		tokens = tokens->next;
		i++;
	}
	return (NULL);
}

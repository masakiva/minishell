#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

ssize_t		error(char *line, t_list **tokens, t_state_machine *machine)
{
	(void)line;
	(void)tokens;
	(void)machine;
	ft_putstr_fd("No ending quote\n", STDERR_FILENO);// ou all->fd[2]?
	return (0);
}

ssize_t		space(char *line, t_list **tokens, t_state_machine *machine)
{
	ssize_t		elem_len;

	(void)tokens;
	elem_len = 0;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '\0')
		machine->state = END;
	else if (ft_isset(*line, ISSPACE_3) == -1)
		machine->state = LETTER;
	else
		elem_len = 1;
	return (elem_len);
}

ssize_t		quote(char *line, t_list **tokens, t_state_machine *machine)
{
	char	quote_style;
	size_t	inside_quotes_len;

	quote_style = *line;
	line++;
	inside_quotes_len = 0;
	while (*line != quote_style && *line != '\0')
	{
		add_to_buf(*line, machine);
		line++;
		inside_quotes_len++;
	}
	if (*line == '\0')
		machine->state = ERR;
	else
		line++;
	if (*line == '\0' || ft_isset(*line, ISSPACE_3) != -1)
	{
		if (link_token(tokens, machine) == ERROR)
			err_bis(MALLOC_ERR);
		if (*line == '\0')
			machine->state = END;
		else
			machine->state = SPACE;
	}
	else if (*line != '"' && *line != '\'')
		machine->state = LETTER;
	return (inside_quotes_len + 2);
}

ssize_t		letter(char *line, t_list **tokens, t_state_machine *machine)
{
	ssize_t		elem_len;

	elem_len = 0;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '\0' || ft_isset(*line, ISSPACE_3) != -1)
	{
		if (link_token(tokens, machine) == ERROR) //si la ligne est vide ou commence par un space on devra retirer le token
			err_bis(MALLOC_ERR);
		if (*line == '\0')
			machine->state = END;
		else
		{
			machine->state = SPACE;
			elem_len = 1;
		}
	}
	else
	{
		add_to_buf(*line, machine);
		elem_len = 1;
	}
	return (elem_len);
}

char			**split_command(char *line)
{
	static t_function	process[NB_STATES] = {letter, quote, space, error};
	t_state_machine		machine;
	ssize_t				elem_len;
	t_list				*tokens;

	errno = 0;
	machine.state = LETTER;
	machine.len = 0;
	machine.cur_token = NULL;
	ft_bzero(&machine.buf, BUF_SIZE);
	tokens = NULL;
	while (*line != '\0')
	{
		elem_len = process[machine.state](line, &tokens, &machine);
		line += elem_len;
		//printf("current = %c ; state = %d\n", *line, machine.state);
	}
	size_t i = 0;
	while (tokens != NULL)
	{
		printf("%zu = %s\n", i, (char *)(tokens->content));
		tokens = tokens->next;
		i++;
	}
	return (NULL);
}

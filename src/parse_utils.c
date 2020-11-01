#include "parsing.h"

#include <stdio.h> // temp
void	print_tokens(t_list *commands)
{
	size_t		i;
	size_t		j;
	t_list		*tokens;
	t_token		*token;
	t_list		*vars;
	t_variable	*var;

	j = 0;
	while (commands != NULL)
	{
		tokens = ((t_command *)commands->content)->tokens;
		printf("################# command %zu: %zu tokens\n", j, ft_lstsize(tokens));
		if (((t_command *)commands->content)->pipe_flag == TRUE)
			printf("PIPE FLAG\n");
		i = 0;
		while (tokens != NULL)
		{
			token = (t_token *)tokens->content;
			printf("%zu = %s\n", i, token->str);
			printf("token len = %zu\n", ft_strlen(token->str));
			if (token->vars != NULL)
			{
				vars = token->vars;
				while (vars != NULL)
				{
					var = vars->content;
					printf("...var starts at %zu ('%c') and ends at %zu ('%c')\n",
							var->start, token->str[var->start],
							var->start + var->len - 1, token->str[var->start + var->len - 1]);
					vars = vars->next;
				}
			}
			if (token->redir > 0)
				printf("...redir %d\n", token->redir);
			tokens = tokens->next;
			i++;
		}
		commands = commands->next;
		j++;
	}
	printf("\n");
}

void		free_content(void *content)
{
	free(content);
}

void		free_token(void *content)
{
	t_token		*token;

	token = (t_token *)content;
	free(token->str);
	ft_lstclear(&token->vars, free_content);
	free(token);
}

void		free_command(void *content)
{
	t_command	*command;

	command = (t_command *)content;
	ft_lstclear(&command->tokens, free_token);
	free(command);
}

void		free_commands(t_list **commands)
{
	ft_lstclear(commands, free_command);
}

int		new_command(t_list **commands)
{
	t_command	*command;
	t_list		*link;

	command = (t_command *)malloc(sizeof(t_command));
	if (command == NULL)
		return (ERROR);
	command->tokens = NULL;
	command->pipe_flag = FALSE;
	link = ft_lstnew(command);
	if (link != NULL)
		ft_lstadd_back(commands, link);
	else
	{
		free(command);
		return (ERROR);
	}
	return (SUCCESS);
}

int		add_variable(t_list **variables, size_t start, size_t end, size_t len)
{
	t_list		*link;
	t_variable	*variable;

	variable = (t_variable *)malloc(sizeof(t_variable));
	if (variable == NULL)
		return (ERROR);
	variable->start = start;
	variable->len = len;
	variable->end = end;
	link = ft_lstnew(variable);
	if (link != NULL)
		ft_lstadd_back(variables, link);
	else
	{
		free(variable);
		return (ERROR);
	}
	return (SUCCESS);
}

char	*parse_variable(char *line, t_state_machine *machine)
{
	size_t		var_start;
	size_t		var_len;
	size_t		var_end;

	var_len = 0;
	if (*line == '?' || ft_isdigit(*line)) // second condition to keep?
	{
		add_to_buf(*line, machine);
		var_len++;
		line++;
	}
	else
	{
		while (ft_isalnum(*line) || *line == '_')
		{
			add_to_buf(*line, machine);
			var_len++;
			line++;
		}
	}
	if (reset_buf(machine) == ERROR)
		return (NULL);
	var_start = ft_strlen(machine->cur_token->str) - var_len;
	var_end = ft_strlen(machine->cur_token->str) - 1;
	if (add_variable(&machine->cur_token->vars, var_start, var_end, var_len) == ERROR)
	{
		free_token(machine->cur_token);
		return (NULL);
	}
	return (line);
}

int		link_token(t_list **tokens, t_state_machine *machine)
{
	t_list	*link;

	if (reset_buf(machine) == ERROR) // laisser en dehors de la func?
		return (ERROR);
	link = ft_lstnew(machine->cur_token);
	if (link != NULL)
		ft_lstadd_back(tokens, link);
	else
	{
		free_token(machine->cur_token);
		return (ERROR);
	}
	machine->cur_token = NULL;
	return (SUCCESS);
}

int		reset_buf(t_state_machine *machine)
{
	char	*res;

	if (machine->cur_token == NULL)
	{
		machine->cur_token = (t_token *)malloc(sizeof(t_token));
		if (machine->cur_token == NULL)
			return (ERROR);
		ft_bzero(machine->cur_token, sizeof(t_token));
		res = ft_strdup(machine->buf); // on a la len de buf
	}
	else
	{
		res = ft_strjoin(machine->cur_token->str, machine->buf); // idem
		free(machine->cur_token->str);
	}
	if (res == NULL)
	{
		ft_lstclear(&machine->cur_token->vars, free_content);
		free(machine->cur_token);
		return (ERROR);
	}
	machine->cur_token->str = res;
	if (machine->len > 0)
	{
		machine->len = 0;
		ft_bzero(&machine->buf, BUF_SIZE);
	}
	return (SUCCESS);
}

int		add_to_buf(char c, t_state_machine *machine)
{
	if (machine->len == BUF_SIZE - 1)
	{
		if (reset_buf(machine) == ERROR)
			return (ERROR);
	}
	machine->buf[machine->len] = c;
	machine->len++;
	return (SUCCESS);
}

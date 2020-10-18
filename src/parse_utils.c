#include "minishell.h"
#include <stdlib.h>

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

void	print_tokens(t_list *tokens)
{
	size_t		i;
	t_token		*token;
	t_list		*vars;
	t_variable	*var;

	i = 0;
	while (tokens != NULL)
	{
		token = (t_token *)tokens->content;
		printf("%zu = %s\n", i, token->str);
		if (token->vars != NULL)
		{
			vars = token->vars;
			while (vars != NULL)
			{
				var = vars->content;
				printf("...var starts at %zu ('%c') and ends at %zu ('%c')\n",
						var->start, token->str[var->start],
						var->end, token->str[var->end]);
				vars = vars->next;
			}
		}
		if (token->redir > 0)
			printf("...redir %d\n", token->redir);
		tokens = tokens->next;
		i++;
	}
}

int		add_variable(t_list **variables, size_t start, size_t end)
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

char	*parse_variable(char *line, t_state_machine *machine)
{
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
		while (ft_isalnum(*line))
		{
			add_to_buf(*line, machine);
			var_len++;
			line++;
		}
	}
	if (reset_buf(machine) == ERROR)
		err_bis(MALLOC_ERR);
	var_end = ft_strlen(machine->cur_token->str) - 1;
	add_variable(&machine->cur_token->vars, var_end - var_len + 1, var_end);
	return (line);
}

int		link_token(t_list **tokens, t_state_machine *machine)
{
	t_list	*link; // "maillon"

	if (reset_buf(machine) == ERROR) // laisser en dehors de la func?
		return (ERROR);
	link = ft_lstnew(machine->cur_token);
	if (link != NULL)
		ft_lstadd_back(tokens, link);
	else
		return (ERROR);
	machine->cur_token = NULL;
	return (SUCCESS);
}

int		reset_buf(t_state_machine *machine) // ou return res
{
	char	*res;

	if (machine->cur_token == NULL)
	{
		machine->cur_token = (t_token *)malloc(sizeof(t_token));
		if (machine->cur_token == NULL)
			return (ERROR);
		machine->cur_token->vars = NULL; // bzero?
		machine->cur_token->redir = NO_REDIR; // bzero?
		res = ft_strdup(machine->buf); // on a la len de buf
	}
	else
	{
		res = ft_strjoin(machine->cur_token->str, machine->buf); // idem
		free(machine->cur_token->str);
	}
	if (res == NULL)
		return (ERROR);
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
	if (machine->len == BUF_SIZE) // ou BUF_SIZE - 1
	{
		if (reset_buf(machine) == ERROR)
			return (ERROR);
	}
	machine->buf[machine->len] = c;
	machine->len++;
	return (SUCCESS);
}

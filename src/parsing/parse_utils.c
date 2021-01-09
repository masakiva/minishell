#include "parsing.h"

//#include <stdio.h> // temp
//void	print_tokens(t_list *commands)
//{
//	size_t		i;
//	size_t		j;
//	t_list		*tokens;
//	t_token		*token;
//	t_list		*vars;
//	t_var_props	*var;
//
//	j = 0;
//	while (commands != NULL)
//	{
//		tokens = ((t_command *)commands->content)->tokens;
//		printf("################# command %zu: %zu tokens\n", j, ft_lstsize(tokens));
//		if (((t_command *)commands->content)->pipe_flag == TRUE)
//			printf("PIPE FLAG\n");
//		i = 0;
//		while (tokens != NULL)
//		{
//			token = (t_token *)tokens->content;
//			printf("%zu = %s\n", i, token->str);
//			printf("token len = %zu\n", ft_strlen(token->str));
//			if (token->var_properties != NULL)
//			{
//				vars = token->var_properties;
//				while (vars != NULL)
//				{
//					var = vars->content;
//					printf("...var starts at %zu ('%c') and ends at %zu ('%c')\n",
//							var->start, token->str[var->start],
//							var->start + var->len - 1, token->str[var->start + var->len - 1]);
//					vars = vars->next;
//				}
//			}
//			if (token->redir > 0)
//				printf("...redir %d\n", token->redir);
//			tokens = tokens->next;
//			i++;
//		}
//		commands = commands->next;
//		j++;
//	}
//	printf("\n");
//}
//
//void		free_content(void *content)
//{
//	free(content);
//}
//
//void		free_token(void *content)
//{
//	t_token		*token;
//
//	token = (t_token *)content;
//	free(token->str);
//	ft_lstclear(&token->var_properties, free_content);
//	free(token);
//}
//
//void		free_command(void *content)
//{
//	t_command	*command;
//
//	command = (t_command *)content;
//	ft_lstclear(&command->tokens, free_token);
//	free(command);
//}
//
//void		free_commands(t_list **commands)
//{
//	ft_lstclear(commands, free_command);
//}
//
//int		new_command(t_list **commands)
//{
//	t_command	*command;
//	t_list		*link;
//
//	command = (t_command *)malloc(sizeof(t_command));
//	if (command == NULL)
//		return (FAILURE);
//	command->tokens = NULL;
//	command->pipe_flag = FALSE;
//	link = ft_lstnew(command);
//	if (link != NULL)
//		ft_lstadd_back(commands, link);
//	else
//	{
//		free(command);
//		return (FAILURE);
//	}
//	return (SUCCESS);
//}
//
//int		add_variable(t_list **var_list, size_t start, size_t len, t_byte split_flag)
//{
//	t_list		*link;
//	t_var_props	*var_props;
//
//	var_props = (t_var_props *)malloc(sizeof(t_var_props));
//	if (var_props == NULL)
//		return (FAILURE);
//	var_props->start = start;
//	var_props->len = len;
//	var_props->split_flag = split_flag;
//	link = ft_lstnew(var_props);
//	if (link != NULL)
//		ft_lstadd_back(var_list, link);
//	else
//	{
//		free(var_props);
//		return (FAILURE);
//	}
//	return (SUCCESS);
//}
//
//char	*parse_variable(char *line, t_state_machine *machine, t_byte split_flag)
//{
//	size_t		var_start;
//	size_t		var_len;
//
//	var_len = 0;
//	if (*line == '?' || ft_isdigit(*line))
//	{
//		add_to_buf(*line, machine);
//		var_len++;
//		line++;
//	}
//	else
//	{
//		while (ft_isalnum(*line) || *line == '_')
//		{
//			add_to_buf(*line, machine);
//			var_len++;
//			line++;
//		}
//	}
//	if (reset_buf(machine) == FAILURE)
//		return (NULL);
//	var_start = ft_strlen(machine->cur_token->str) - var_len;
//	if (add_variable(&machine->cur_token->var_properties, var_start, var_len,
//				split_flag) == FAILURE)
//	{
//		free_token(machine->cur_token);
//		return (NULL);
//	}
//	return (line);
//}

int		add_arg(t_state_machine *machine)
{
	if (reset_buf(machine) == FAILURE)
		return (FAILURE);
	machine->args = push_str_to_array(machine->args, machine->cur_arg);
	if (machine->args == NULL)
	{
		free(machine->cur_arg);
		return (FAILURE);
	}
	machine->cur_arg = NULL;
	return (SUCCESS);
}

int		reset_buf(t_state_machine *machine)
{
	if (machine->cur_arg == NULL)
		machine->cur_arg = ft_strdup(machine->buf); // on a la len de buf
	else
		machine->cur_arg = strjoin_free(machine->cur_arg, machine->buf); // idem
	if (machine->cur_arg == NULL)
		return (FAILURE);
	if (machine->len > 0)
	{
		machine->len = 0;
		ft_bzero(&machine->buf, BUF_SIZE);
	}
	return (SUCCESS);
}

int		add_to_buf(t_state_machine *machine, char c)
{
	if (machine->len == BUF_SIZE - 1)
	{
		if (reset_buf(machine) == FAILURE)
			return (FAILURE);
	}
	machine->buf[machine->len] = c;
	machine->len++;
	return (SUCCESS);
}

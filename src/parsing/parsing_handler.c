/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 11:25:06 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/01 11:25:09 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int			parse_input(char **line, char **env, int stat_loc,
		t_command *command)
{
	static t_parse	process[NB_STATES - 1] = {space, letter, backslash, dollar,
		single_quote, double_quote, quoted_backslash, quoted_dollar,
		angle_bracket};
	t_state_machine	machine;

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
	free(machine.cur_arg);
	command->pipe_flag = machine.pipe_flag;
	command->redir_paths = machine.redir_paths;
	command->redir_types = machine.redir_types;
	command->args = machine.args;
	if (*line == NULL)
		return (FAILURE);
	return (SUCCESS);
}

t_command	*parse_one_command(t_xe *xe)
{
	t_command	*cur_command;
	int			ret;

	cur_command = (t_command *)malloc(sizeof(t_command));
	if (cur_command == NULL)
		return (NULL);
	ret = parse_input(&xe->line, xe->env, xe->stat_loc, cur_command);
	if (ret == FAILURE)
	{
		free_command(cur_command);
		return (NULL);
	}
	return (cur_command);
}

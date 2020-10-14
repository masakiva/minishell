/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:21:17 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/10/14 02:19:14 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

int		error(char *line, t_state_machine *machine)
{
	(void)line;
	(void)machine;
	return (0);
}

int		end(char  *line, t_state_machine *machine)
{
	(void)line;
	machine->state = END;
	return (0);
}

int		double_quote(char *line, t_state_machine *machine)
{
	size_t	len;

	len = 0;
	while (line[len] != '"' && line[len] != '\0')
		len++;
	if (line[len] != '\0')
	{
		ft_substr(line, 0, len);
		machine->state = NOQUOTE;
	}
	else
		machine->state = ERR;
	return (len + 1);
}
#include <stdio.h>

int		single_quote(char *line, t_state_machine *machine)
{
	size_t	len;
	char	*contents;

	len = 0;
	while (line[len] != '\'' && line[len] != '\0')
		len++;
	if (line[len] != '\0')
	{
		contents = ft_substr(line, 0, len);
		printf("len = %zu, str = %s\n", len, contents);
		free(contents);
		machine->state = NOQUOTE;
	}
	else
		machine->state = ERR;
	return (len + 1);
}

int		noquote(char *line, t_state_machine *machine)
{
	if (*line == '"')
		machine->state = DOUBLE_QUOTE;
	else if (*line == '\'')
	{
		machine->state = SINGLE_QUOTE;
		printf("SINGLE\n");
	}
	else
		machine->state = END;
	return (1);
}

char			**split_command(char *line)
{
	static t_function	process[NB_STATES] = {noquote, single_quote, double_quote, error};
	t_state_machine		machine;
	size_t				elem_len;

	machine.state = NOQUOTE;
	while (*line != '\0')
	{
		printf("current = %c\n", *line);
		elem_len = process[machine.state](line, &machine);
		line += elem_len;
		if (machine.state == END)
			break ;
	}
	return (NULL);
}

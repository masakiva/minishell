#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "execution.h"

#include <stdio.h>

static int			get_input(t_list **commands)
{
	char	*line;
	int		ret;

	if (isatty(STDIN_FILENO)) // temp
		ft_putstr_fd(PROMPT, STDOUT_FILENO); // err
	ret = get_next_line(STDIN_FILENO, &line);
	if (ret == ERROR) // defined for gnl
		return (MALLOC_ERR); // or read(2) error
	else if (ret == 0) // EOF in files and heredocs (noeol files not yet supported)
	{
		free(line);
		free_commands(commands);
		return (CLEAN_EXIT);
	}
	//if (line[0] == '\0')
		//ft_putstr_fd("\n", 1);
	*commands = parse_input(line);
	if (*commands == NULL)
		return (MALLOC_ERR);
	free(line);
	return (SUCCESS);
}

static int			main_loop(char **env)
{
	int			ret;
	t_list		*commands;
	t_command	*cur_command;
	char		**args;

	ret = get_input(&commands);
	if (ret != SUCCESS)
		;//err
	while (commands != NULL && ret == SUCCESS)
	{
		cur_command = ft_lstpop(&commands);
		if (cur_command != NULL) // le contraire possible?
		{
			args = prepare_args(cur_command, env);
			ret = execute_cmd(args, env);
		}
	}
	return (ret);
}

int					main(int argc, char **argv, char **env)
{
	int				ret;

	(void)argv;
	if (argc != 1)
		ret = ARG_ERR;
	else
	{
		signal_handler();
		ret = SUCCESS;
		while (ret == SUCCESS)
			ret = main_loop(env);
	}
	return (ft_exit(ret, env));
}

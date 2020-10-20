#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*static*/ enum e_command	get_command_code(char *arg)
{
	char	**command;
	int		i;

	i = 0;
	command = ft_split(BUILTINS, '/');
	if (command == NULL)
		return (MALLOC_ERR);
	while (command[i] != NULL)
	{
		if (ft_strcmp(arg, command[i]) == 0)
		{
			free_str_array(&command);
			return (i);
		}
		i++;
	}
	free_str_array(&command);
	return (ELSE);
}

void	sig_int(int signum)
{
	(void)signum;
	ft_putstr_fd("SIGINT", 1);
	ft_putstr_fd("\n", 1);
	exit(0);
}

void	sig_kill(int signum)
{
	(void)signum;
	ft_putstr_fd("SIGQUIT", 1);
	ft_putstr_fd("\n", 1);
	exit(0);
}

static int			get_input(t_all *all)
{
	int		ret;
	char	*line;

	ret = 1;
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_kill);
	ft_putstr_fd(PROMPT, all->fd[1]); // err
	ret = get_next_line(all->fd[0], &line);
	if (line == NULL)
		return (MALLOC_ERR);
	if (line[0] == '\0')
	{
		ft_putstr_fd("\n", 1);
	}
//	parse_input(line);;
	all->current = ft_split(line, ' ');
	free(line);
//	if (all->current == NULL)
//		return (MALLOC_ERR);
	all->command = get_command_code(all->current[0]);
	if (ret == 1)
		return (SUCCESS);
	else
		return (CLEAN_EXIT);
}

static int			main_loop(t_all *all)
{
	int	ret;

	if ((ret = get_input(all)) != SUCCESS)
		return (ret);
	if ((ret = launch_command(all)) != SUCCESS)
		return (ret);
	return (ret);
}

int					main(int argc, char **argv, char **env)
{
	t_all			all;
	int				ret;

	(void)argv;
	if (argc != 1)
	{
		all.current = NULL;
		all.env = NULL;
		return (ft_exit(ARG_ERR, &all));
	}
	else
	{
		all.current = env;
		if ((ret = all_init(&all)) != SUCCESS)
			return (ft_exit(ret, &all));
	}
	ret = SUCCESS;
	while (ret == SUCCESS)
		ret = main_loop(&all);
	return (ft_exit(ret, &all));
}

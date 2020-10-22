#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

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

void				signal_handler(void)
{
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_kill);
}

static int			get_input(t_all *all)
{
	int		ret;
	char	*line;

	ret = 1;
//	signal_handler();
	ft_putstr_fd(PROMPT, STDOUT_FILENO); // err
	ret = get_next_line(STDIN_FILENO, &line);
	if (ret == ERROR)
		return (MALLOC_ERR); // or read(2) error
//	if (line[0] == '\0')
//		ft_putstr_fd("\n", 1);//?
	all->commands = parse_input(line);
	if (all->commands == NULL)
		return (MALLOC_ERR);
	free(line);
	parse_commands(&all->commands, &all->env);
	free_commands(&all->commands);
//	if (ret == 1)
		return (SUCCESS);
//	else
//		return (CLEAN_EXIT);
}

static int			main_loop(t_all *all)
{
	int	ret;

	if ((ret = get_input(all)) != SUCCESS)
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

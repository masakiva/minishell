#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void	sig_int(int signum)
{
	//	ft_putstr_fd("SIGINT", 1);
	ft_putstr_fd("\n", STDOUT_FILENO);
	//	kill(0, signum);
	exit(signum);
	//	signal(signum, SIG_DFL);
}

void	sig_kill(int signum)
{
	//	ft_putstr_fd("SIGQUIT", 1);
	ft_putstr_fd("\n", STDOUT_FILENO);
	//	kill(0, signum);
	exit(signum);
	//	signal(signum, SIG_DFL);
}

void				signal_handler(void)
{
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_kill);
}

static int			get_input(t_list **commands)
{
	char	*line;
	int		ret;

	ft_putstr_fd(PROMPT, STDOUT_FILENO); // err
	ret = get_next_line(STDIN_FILENO, &line);
	if (ret == ERROR)
		return (MALLOC_ERR); // or read(2) error
	//if (line[0] == '\0')
		//ft_putstr_fd("\n", 1);
	*commands = parse_input(line);
	if (*commands == NULL)
		return (MALLOC_ERR);
	free(line);
	if (ret == 1)
		return (SUCCESS);
	else // dans quel cas gnl renvoie 0?
		return (CLEAN_EXIT);
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
			//free_command(cur_command);
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
		//ret = all_init(&all);
		ret = SUCCESS;
		if (ret == SUCCESS)
		{
			signal_handler();
			while (ret == SUCCESS)
				ret = main_loop(env);
		}
	}
	return (ft_exit(ret, env));
}

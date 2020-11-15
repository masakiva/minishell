#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid

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

static int			handle_commands(t_list **commands, char **env)
{
	int			ret;
	char		**args;
	t_command	*cur_command;

	ret = SUCCESS;
	gpid = -1;
	cur_command = NULL;
	if (commands != NULL)
		cur_command = ft_lstpop(commands);
	if (cur_command != NULL) // le contraire possible?
	{
		if (cur_command->pipe_flag == TRUE)
		{
			pipe(fd_pipe);
			gpid = fork();
			fd_backup = dup(0);
			if (gpid == 0)
			{
				fd_old = STDOUT_FILENO;
				dup2(fd_old, fd_backup);
				dup2(fd_pipe[1], fd_old);
				close(fd_pipe[0]);
				args = prepare_args(cur_command, env);
				ret = execute_cmd(args, env);
				close(fd_pipe[1]);
				return (CLEAN_EXIT);
			}
			else
			{
				fd_old = STDIN_FILENO;
				dup2(fd_old, fd_backup);
				dup2(fd_pipe[0], fd_old);
				free(cur_command);
				close(fd_pipe[0]);
				close(fd_pipe[1]);
				cur_command = ft_lstpop(commands);
				if (cur_command != NULL)
				{
					args = prepare_args(cur_command, env);
					ret = execute_cmd(args, env);
					signal(SIGINT, SIG_IGN);
					signal(SIGQUIT, SIG_IGN);
					waitpid(gpid, &stat_loc, 0);
					signal_handler();
				}
			}
		}
		else
		{
			args = prepare_args(cur_command, env);
			ret = execute_cmd(args, env);
		}
	}
	return (ret);
}

static int			main_loop(char **env)
{
	int			ret;
	t_list		*commands;
//	t_command	*cur_command;
//	char		**args;

	fd_backup = -1;
	ret = get_input(&commands);
	if (ret != SUCCESS)
		;//err
	ret = handle_commands(&commands, env);
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

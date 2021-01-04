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

	if (isatty(STDIN_FILENO)) // temp pour le testeur
	{
		if (ft_putstr_fd(PROMPT, STDOUT_FILENO) == ERROR)
			return (WRITE_ERR);
	}
	ret = get_next_line(STDIN_FILENO, &line);
	if (ret == ERROR)
		return (GNL_ERR);
	else if (ret == 0) // EOF in files and heredocs (noeol files not yet supported)
	{
		free(line);
		free_commands(commands);
		return (CLEAN_EXIT);
	}
	*commands = parse_input(line);
	free(line);
	if (*commands == NULL)
		return (MALLOC_ERR);
	return (SUCCESS);
}

static int			handle_execution(t_xe *xe)
{
	int			ret;
	char		**args;
	t_command	*cur_command;

	ret = SUCCESS;
	gpid = -1;
	cur_command = NULL;
	if (xe->commands != NULL)
		cur_command = ft_lstshift(&(xe->commands));
	if (cur_command != NULL) // le contraire possible?
	{
		if (cur_command->pipe_flag == TRUE)
		{
			pipe(fd_pipe);// error?
			gpid = fork();// error?
			if (gpid == 0)
			{
				close(fd_pipe[0]);// error
				dup2(fd_pipe[1], STDOUT_FILENO);// error
				args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
				ret = execute_cmd(args, xe);// error
				close(fd_pipe[1]);// error
				return (CLEAN_EXIT);
			}
			else
			{
				close(fd_pipe[1]);// error
				dup2(fd_pipe[0], STDIN_FILENO);// error
				free(cur_command);
				ret = handle_execution(xe);
				close(fd_pipe[0]);// error
				return (ret);
/*
				cur_command = ft_lstshift(&(xe->commands));
				if (cur_command != NULL)// else error
				{
					args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
					ret = execute_cmd(args, xe);// error
					signal(SIGINT, SIG_IGN);// error?
					signal(SIGQUIT, SIG_IGN);// error?
					waitpid(gpid, &xe->stat_loc, 0);// error?
					signal_handler();
				}
*/
			}
		}
		else
		{
			args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
			ret = execute_cmd(args, xe);// error
			return (ret);
		}
	}
	else
	{
		dup2(backup_stdin, STDIN_FILENO);
		dup2(backup_stdout, STDOUT_FILENO);
		return (SUCCESS);
	}
}

static int			main_loop(t_xe *xe)
{
	int			ret;

	ret = get_input(&xe->commands);
	if (ret == SUCCESS)
		ret = handle_execution(xe);
	return (ret);
}

int		main(int argc, char **argv, char **env_source)
{
	int		ret;
	t_xe 	*xe;

	(void)argv;
	ret = SUCCESS;
	xe = (t_xe *)malloc(sizeof(t_xe));
	ft_bzero(xe, sizeof(t_xe));
	backup_stdin = dup(STDIN_FILENO);
	backup_stdout = dup(STDOUT_FILENO);
	if (xe == NULL)
		ret = MALLOC_ERR;
	else if (argc != 1)
	{
		ret = ARG_ERR;
		free(xe);
	}
	else
	{
		signal_handler(); // err?
		xe->env = dup_str_array(env_source);
		if (xe->env == NULL)
			ret = MALLOC_ERR;
		while (ret == SUCCESS)
			ret = main_loop(xe);
		free_str_array(xe->exported); // besoin de free?
		free_str_array(xe->env); // besoin de free?
		free(xe);
	}
	return (ft_exit(ret, xe));
}

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
		cur_command = ft_lstpop(&(xe->commands));
	if (cur_command != NULL) // le contraire possible?
	{
		if (cur_command->pipe_flag == TRUE)
		{
			pipe(fd_pipe);// error?
			gpid = fork();// error?
			fd_backup = dup(0);
			if (gpid == 0)
			{
				fd_old = STDOUT_FILENO;
				dup2(fd_old, fd_backup);// error
				dup2(fd_pipe[1], fd_old);// error
				close(fd_pipe[0]);// error
				args = prepare_args(cur_command, xe->env);// error
				ret = execute_cmd(args, xe->env);// error
				close(fd_pipe[1]);// error
				ret = CLEAN_EXIT;
				//return (CLEAN_EXIT);
			}
			else
			{
				fd_old = STDIN_FILENO;
				dup2(fd_old, fd_backup);// error
				dup2(fd_pipe[0], fd_old);// error
				free(cur_command);
				close(fd_pipe[0]);// error
				close(fd_pipe[1]);// error
				cur_command = ft_lstpop(&(xe->commands));
				if (cur_command != NULL)// else error
				{
					args = prepare_args(cur_command, xe->env);// error
					ret = execute_cmd(args, xe->env);// error
					signal(SIGINT, SIG_IGN);// error?
					signal(SIGQUIT, SIG_IGN);// error?
					waitpid(gpid, &stat_loc, 0);// error?
					signal_handler();
				}
			}
		}
		else
		{
			args = prepare_args(cur_command, xe->env);// error
			ret = execute_cmd(args, xe->env);// error
		}
	}
	return (ret);
}

static int			main_loop(t_xe *xe)
{
	int			ret;

	fd_backup = -1;
	ret = get_input(&(xe->commands));
//	write(1, "OK\n", 3);
	if (ret == SUCCESS)
		ret = handle_execution(xe);
	return (ret);
}

char	**extract_env(char	**env_source)
{
	int		i;
	char	**ret;

	if (!(ret = malloc((ft_arraylen(env_source) + 1) * sizeof(char*))))
		return (NULL);
	i = 0;
	while (env_source[i])
	{
		ret[i] = ft_strdup(env_source[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

int					main(int argc, char **argv, char **env_source)
{
	int				ret;
	t_xe 			*xe;

	(void)argv;
	xe = malloc(sizeof(t_xe));
	if (xe == NULL)
		return (ft_exit(MALLOC_ERR, xe));
	if (argc != 1)
		ret = ARG_ERR;
	else
	{
		signal_handler();
		xe->env = extract_env(env_source);
		ret = SUCCESS;
		while (ret == SUCCESS)
			ret = main_loop(xe);
	}
	return (ft_exit(ret, xe));
}

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

int			handle_execution(t_xe *xe)
{
	int			fd[2];
	int			ret;
	char		**args;
	t_command	*cur_command;

	ret = SUCCESS;
	xe->gpid = -1;
	cur_command = NULL;
	if (xe->commands != NULL)
		cur_command = ft_lstshift(&(xe->commands));
	if (cur_command != NULL) // le contraire possible?
	{
		if (((t_list *)cur_command->tokens) == NULL)
		{
			free_command(cur_command);
			cur_command = ft_lstshift(&(xe->commands));
			return (SUCCESS);
		}
		else if (cur_command->pipe_flag == TRUE)
		{
			pipe(fd);// error?
			xe->gpid = fork();// error?
			if (xe->gpid == 0)
			{
				//xe->child = 1;
				close(fd[0]);// error
				dup2(fd[1], STDOUT_FILENO);// error
				//dprintf(xe->backup_stdout, "list: child = %s\n", ((t_token *)((t_list *)cur_command->tokens)->content)->str);
				args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
				//dprintf(xe->backup_stdout, "args0: child = %s\n", args[0]);
				//dprintf(xe->backup_stdout, "args1: child = %s\n", args[1]);
				ret = execute_cmd(args, xe);// error
				close(fd[1]);
				return (CLEAN_EXIT);
			}
			else
			{
				//xe->child = 0;
				close(fd[1]);// error
				dup2(fd[0], STDIN_FILENO);
			//	free_command(cur_command);
				cur_command = ft_lstshift(&(xe->commands));
				if (cur_command == NULL)
					return (MALLOC_ERR);
				//dprintf(xe->backup_stdout, "list: parent = %s\n", ((t_token *)((t_list *)cur_command->tokens)->content)->str);
				args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
				//dprintf(xe->backup_stdout, "args0: parent = %s\n", args[0]);
				//dprintf(xe->backup_stdout, "args1: parent = %s\n", args[1]);
				ret = execute_cmd(args, xe);// error
				close(fd[0]);
				dup2(xe->backup_stdout, STDOUT_FILENO);
				dup2(xe->backup_stdin, STDIN_FILENO);
				return (ret);
//				return (handle_execution(xe, fd[0], proc + 1));
			}
		}
		else
		{
			//dup2(fd_in, STDIN_FILENO);
			args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
			ret = execute_cmd(args, xe);// error
			return (ret);
		}
	}
	else
	{
		dup2(xe->backup_stdout, STDOUT_FILENO);
		dup2(xe->backup_stdin, STDIN_FILENO);
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
	xe->backup_stdin = dup(STDIN_FILENO);
	xe->backup_stdout = dup(STDOUT_FILENO);
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

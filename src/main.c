#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "pre_execution.h"
#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid


static int			get_input(char **line)
{
	int		ret;

	if (isatty(STDIN_FILENO)) // temp pour le testeur
	{
		if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
			return (WRITE_ERR);
	}
	ret = get_next_line(STDIN_FILENO, line);
	if (ret == ERROR)
		return (GNL_ERR);
	else if (ret == 0) // EOF in files and heredocs (noeol files not yet supported)
	{
		free(*line);
		return (CLEAN_EXIT);
	}
	return (ret);
}

int			handle_execution(t_xe *xe, int fd_in, int proc)
{
	int			i;
	int			fd[2];
	int			ret;
	t_command	*cur_command;

	ret = SUCCESS;
	xe->gpid = -1;
	cur_command = NULL;
	if (xe->commands != NULL)
		cur_command = ft_lstshift(&(xe->commands));
	if (cur_command != NULL) // le contraire possible?
	{
		if ((cur_command->args) == NULL)
		{
			free_command(cur_command);
		//	cur_command = ft_lstshift(&(xe->commands));
		//	il faudrait plutôt partir sur une recursion ici
			dup2(xe->backup_stdout, STDOUT_FILENO);
			dup2(xe->backup_stdin, STDIN_FILENO);
			return (SUCCESS);
		}
		else if (cur_command->pipe_flag == TRUE)
		{
			pipe(fd);// error?
			xe->gpid = fork();// error?
			if (xe->gpid == 0)
			{
				//xe->child = 1;
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				close(fd[0]);// error
			//	dup2(fd_in, STDIN_FILENO);
				if (fd_in != STDIN_FILENO)
				{
					if (dup2(fd_in, STDIN_FILENO) != -1)
						close(fd_in);
				}
		//		dup2(fd[1], STDOUT_FILENO);
				if (dup2(fd[1], STDOUT_FILENO) != -1)// error
					close(fd[1]);
				//dprintf(xe->backup_stdout, "list: child = %s\n", ((t_token *)((t_list *)cur_command->tokens)->content)->str);
				//dprintf(xe->backup_stdout, "args0: child = %s\n", args[0]);
				//dprintf(xe->backup_stdout, "args1: child = %s\n", args[1]);
				ret = execute_cmd(cur_command->args, xe);// error
				return (CLEAN_EXIT);
			}
			else
			{
				//xe->child = 0;
				close(fd[1]);// error
				close(fd_in);
				free_command(cur_command);
				return (handle_execution(xe, fd[0], proc + 1));
			}
		}
		else
		{
			if (fd_in != STDIN_FILENO)
			{
				if (dup2(fd_in, STDIN_FILENO) != -1)
					close(fd_in);
			}
			ret = execute_cmd(cur_command->args, xe);// error
			dup2(xe->backup_stdout, STDOUT_FILENO);
			dup2(xe->backup_stdin, STDIN_FILENO);
			i = 0;
			while (i < proc)
			{
				wait(NULL);
				i++;
			}
			if (ret == SUCCESS)
				return (handle_execution(xe, STDIN_FILENO, 0));
			else
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
	char		*line;

	ret = get_input(&line);
	if (ret == SUCCESS)
	{
		//check_syntax(line);
		parse_commands(xe, line);
		ret = handle_execution(xe, STDIN_FILENO, 0);
		free(line);
	}
	return (ret);
}

int		exec_env_init(t_xe *xe, char **env_source)
{
	char		*shlvl;
	char		*val;
	int			tmp;

	xe->env = dup_str_array(env_source);
	shlvl = get_var_value(xe->env, SHLVL_STR, ft_strlen(SHLVL_STR));
//	printf("shlvl = %s\n", shlvl);
	tmp = ft_atoi(shlvl);
	tmp += 1;
	val = ft_itoa(tmp);
	env_replace_var(SHLVL_STR, val, xe);
	return (0);
}

int		main(int argc, char **argv, char **env_source)
{
	int		ret;
	t_xe 	*xe;

	(void)argv;
	ret = SUCCESS;
	xe = (t_xe *)malloc(sizeof(t_xe));
	if (xe == NULL)
		return (ft_exit(MALLOC_ERR, xe));
	ft_bzero(xe, sizeof(t_xe));
	if (xe == NULL)
		ret = MALLOC_ERR;
	else if (argc != 1)
	{
		ret = ARG_ERR;
		free(xe);
	}
	else
	{
		xe->backup_stdin = dup(STDIN_FILENO);
		xe->backup_stdout = dup(STDOUT_FILENO);
		signal_handler(); // err?
		exec_env_init(xe, env_source);
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

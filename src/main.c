#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "pre_execution.h"
#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid

static int			get_input(t_list **commands)
{
	char	*line;
	int		ret;

	if (isatty(STDIN_FILENO)) // temp pour le testeur
	{
		if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
			return (WRITE_ERR);
	}
	ret = get_next_line(STDIN_FILENO, &line);
	if (ret == ERROR)
		return (GNL_ERR);
	else if (ret == 0) // EOF in files and heredocs (noeol files not yet supported)
	{
		free(line);
		return (CLEAN_EXIT);
	}
	ret = parse_input(line, commands);
	free(line);
	return (ret);
}

int			handle_execution(t_xe *xe, int fd_in, int proc)
{
	int			i;
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
				args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
				//dprintf(xe->backup_stdout, "args0: child = %s\n", args[0]);
				//dprintf(xe->backup_stdout, "args1: child = %s\n", args[1]);
				ret = execute_cmd(args, xe);// error
				return (CLEAN_EXIT);
			}
			else
			{
				//xe->child = 0;
				close(fd[1]);// error
				close(fd_in);
				free_command(cur_command);
/*
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
*/
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
			args = prepare_args(cur_command, xe->env, xe->stat_loc);// error
			ret = execute_cmd(args, xe);// error
			dup2(xe->backup_stdout, STDOUT_FILENO);
			dup2(xe->backup_stdin, STDIN_FILENO);
			i = 0;
			while (i < proc)
			{
				wait(NULL);
				i++;
			}
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
	(void)signum;
	(void)signum;
	if (ret == SUCCESS)
		ret = handle_execution(xe, STDIN_FILENO, 0);
	else if (ret == PARSING_ERR)
		ret = SUCCESS;
	return (ret);
}

#include <stdio.h>

int		exec_env_init(t_xe *xe, char **env_source)
{
	char		*shlvl;
	char		**new;
	char		*val;
	int			tmp;
	int			i;
	int			j;

	xe->env = dup_str_array(env_source);
	shlvl = get_var_value(xe->env, SHLVL_STR);
//	printf("shlvl = %s\n", shlvl);
	tmp = ft_atoi(shlvl);
	tmp += 1;
	val = ft_itoa(tmp);
	new = malloc(sizeof(char*) * 3);
	new[0] = ft_strdup("export");
	new[1] = malloc(sizeof(char) * (ft_strlen(SHLVL_STR) + ft_strlen(val) + 2));
	new[2] = NULL;
	if (new == NULL)
		return (MALLOC_ERR);
	i = 0;
	while (SHLVL_STR[i] != '\0')
	{
		new[1][i] = SHLVL_STR[i];
		i++;
	}
	new[1][i] = '=';
	i += 1;
	j = 0;
	while (val[j] != '\0')
	{
		new[1][i + j] = val[j];
		j++;
	}
	new[1][i + j] = '\0';
//	printf("shlvl = %d\n", tmp);
	ft_export(new, xe);
	return (0);
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

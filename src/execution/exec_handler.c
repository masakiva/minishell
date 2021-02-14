/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 17:52:41 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/14 22:06:43 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "execution.h"

static int	child_pipe(t_command *cur_command, const int *fd,
							int fd_in, t_xe *xe)
{
	int		ret;

	xe->flags += CHILD;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (close(fd[0]) == ERROR)
		return (FAILURE);
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) == ERROR || close(fd_in) == ERROR)
			return (FD_ERROR);
	}
	if (dup2(fd[1], STDOUT_FILENO) == ERROR || close(fd[1]) == ERROR)
		return (FD_ERROR);
	ret = execute_cmd(cur_command->args, cur_command->redir_paths,
						cur_command->redir_types, xe);
	free_command(cur_command);
	xe->flags -= RUN;
	return (SUCCESS);
}

int			handle_pipe(t_command *cur_command, t_xe *xe, int fd_in, int proc)
{
	int			fd[2];
	int			ret;

	ret = SUCCESS;
	if (pipe(fd) == ERROR)
		return (FD_ERROR);
	xe->gpid = fork();
	if (xe->gpid == ERROR)
		return (FORK_ERROR);
	else if (xe->gpid == 0)
		return (child_pipe(cur_command, fd, fd_in, xe));
	else
	{
		if (!(xe->flags & CMD_PIPE))
			xe->flags += CMD_PIPE;
		if (close(fd[1]) == ERROR || close(fd_in) == ERROR)
			return (FD_ERROR);
		free_command(cur_command);
		return (handle_exec_return(xe, fd[0], proc + 1));
	}
}

int			handle_command(t_command *cur_command, t_xe *xe,
								int fd_in, int proc)
{
	int			ret;

	ret = SUCCESS;
	if (cur_command->args == NULL)
	{
		free_command(cur_command);
		dup2(xe->backup_stdout, STDOUT_FILENO);
		dup2(xe->backup_stdin, STDIN_FILENO);
		return (handle_exec_return(xe, STDIN_FILENO, 0));
	}
	else if (cur_command->pipe_flag == TRUE)
	{
		return (handle_pipe(cur_command, xe, fd_in, proc));
	}
	else
	{
		return (parent_pipe_end(cur_command, xe, fd_in, proc));
	}
}

static int	check_redir_type(t_command *cur_command, t_xe *xe,
								int fd_in, int proc)
{
	if (find_ambig_redir(cur_command->redir_paths,
							cur_command->redir_types) != SUCCESS)
	{
		free_command(cur_command);
		if (ft_error(AMBIG_REDIR, xe) != SUCCESS)
			return (FAILURE);
		dup2(xe->backup_stdout, STDOUT_FILENO);
		dup2(xe->backup_stdin, STDIN_FILENO);
		xe->stat_loc = 1;
		return (handle_exec_return(xe, fd_in, proc));
	}
	if (cur_command->args == NULL)
	{
		if (apply_redirs(cur_command->redir_paths,
							cur_command->redir_types, xe) != SUCCESS)
		{
			free_command(cur_command);
			return (FD_ERROR);
		}
		free_command(cur_command);
		return (handle_exec_return(xe, fd_in, proc));
	}
	return (KEEP_ON);
}

int			handle_execution(t_xe *xe, int fd_in, int proc)
{
	int			ret;
	t_command	*cur_command;

	xe->gpid = -1;
	cur_command = parse_one_command(xe);
	if (cur_command == NULL)
		return (MALLOC_ERR);
	if (cur_command->redir_types != NULL)
	{
		ret = check_redir_type(cur_command, xe, fd_in, proc);
		if (ret != KEEP_ON)
			return (ret);
	}
	if (cur_command->args != NULL || cur_command->var_flag == TRUE)
		return (handle_command(cur_command, xe, fd_in, proc));
	else
	{
		free_command(cur_command);
		dup2(xe->backup_stdout, STDOUT_FILENO);
		dup2(xe->backup_stdin, STDIN_FILENO);
		return (SUCCESS);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:10:57 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:14:59 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	signal_interupt(t_xe *xe)
{
	xe->stat_loc = WTERMSIG(xe->stat_loc);
	if (xe->stat_loc == SIGQUIT)
	{
		if (ft_putstr_fd("\b\b^\\Quit (core dumped)\n", STDERR_FILENO)
				!= WRITE_SUCCESS)
			return (WRITE_ERR);
	}
	else if (xe->stat_loc == SIGINT)
	{
		if (ft_putstr_fd("\n", STDERR_FILENO) != WRITE_SUCCESS)
			return (WRITE_ERR);
	}
	xe->stat_loc = (xe->stat_loc % 256) + 128;
	return (SUCCESS);
}

static int	parent_wait(t_xe *xe, int proc)
{
	int			i;
	int			ret;

	i = 0;
	while (i < proc)
	{
		ret = wait(&xe->stat_loc);
		if (ret == ERROR)
			return (WAIT_ERROR);
		if (WIFSIGNALED(xe->stat_loc))
		{
			ret = signal_interupt(xe);
			if (ret != SUCCESS)
				return (ret);
		}
		else if (WIFEXITED(xe->stat_loc))
			xe->stat_loc = WEXITSTATUS(xe->stat_loc);
		i++;
	}
	return (SUCCESS);
}

static int	parent_pipe_exec(t_command *cur_command, t_xe *xe,
								int fd_in, int *tmp)
{
	int	ret;

	ret = SUCCESS;
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) == ERROR || close(fd_in) == ERROR)
			return (FD_ERROR);
	}
	if (dup2(xe->backup_stdout, STDOUT_FILENO) == ERROR)
		return (FD_ERROR);
	ret = execute_cmd(cur_command->args, cur_command->redir_paths,
						cur_command->redir_types, xe);
	*tmp = xe->stat_loc;
	free_command(cur_command);
	if (dup2(xe->backup_stdout, STDOUT_FILENO) == ERROR)
		return (FD_ERROR);
	if (dup2(xe->backup_stdin, STDIN_FILENO) == ERROR)
		return (FD_ERROR);
	return (ret);
}

int			parent_pipe_end(t_command *cur_command, t_xe *xe,
								int fd_in, int proc)
{
	int			ret;
	int			tmp;
	int			wait_ret;

	ret = SUCCESS;
	ret = parent_pipe_exec(cur_command, xe, fd_in, &tmp);
	wait_ret = parent_wait(xe, proc);
	if (wait_ret != SUCCESS)
		return (wait_ret);
	if (xe->flags & EXIT_FLAG || (ret != SUCCESS && ret < _ERRNO_MSG_))
		return (ret);
	else
	{
		xe->stat_loc = tmp;
		ft_error(ret, xe);
		if (ret == EXIT_ARG_ERR)
			return (SUCCESS);
		if (!(xe->flags & INTERUPT))
			return (handle_execution(xe, STDIN_FILENO, 0));
		else
		{
			xe->flags = RUN;
			return (SUCCESS);
		}
	}
}

#include "parsing.h"
#include "execution.h"

static int		child_setup(const int *fd, int fd_in, t_xe *xe)
{
	xe->flags += CHILD;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (close(fd[0]) == ERROR)
		return (FAILURE);
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) == ERROR || close(fd_in) == ERROR)
			return (FAILURE);
	}
	if (dup2(fd[1], STDOUT_FILENO) == ERROR || close(fd[1]) == ERROR)
		return (FAILURE);
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
	{
		if (child_setup(fd, fd_in, xe) != SUCCESS)
			return (FD_ERROR);
		ret = execute_cmd(cur_command->args, cur_command->redir_paths, cur_command->redir_types, xe);
		free_command(cur_command);
		xe->flags -= RUN;
		return (ret);
	}
	else
	{
		if (!(xe->flags & CMD_PIPE))
			xe->flags += CMD_PIPE;
		if (close(fd[1]) == ERROR || close(fd_in) == ERROR)
			return (FD_ERROR);
		free_command(cur_command);
		return (handle_execution(xe, fd[0], proc + 1));
	}
}

int			parent_pipe_end(t_command *cur_command, t_xe *xe, int fd_in, int proc)
{
	int			i;
	int			ret;
	int			tmp;
	int			wait_ret;

	ret = SUCCESS;
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) == ERROR || close(fd_in) == ERROR)
			return (FD_ERROR);
	}
	if (dup2(xe->backup_stdout, STDOUT_FILENO) == ERROR)
		return (FD_ERROR);
	ret = execute_cmd(cur_command->args, cur_command->redir_paths, cur_command->redir_types, xe);
	tmp = xe->stat_loc;
	free_command(cur_command);
	if (dup2(xe->backup_stdout, STDOUT_FILENO) == ERROR)
		return (FD_ERROR);
	if (dup2(xe->backup_stdin, STDIN_FILENO) == ERROR)
		return (FD_ERROR);
	i = 0;
	while (i < proc)
	{
		wait_ret = wait(&xe->stat_loc);
		if (wait_ret == ERROR)
			return (WAIT_ERROR);
		if (WIFSIGNALED(xe->stat_loc))
		{
			xe->stat_loc = WTERMSIG(xe->stat_loc);
			if (xe->stat_loc == SIGQUIT)
			{
				if (ft_putstr_fd("\b\b^\\Quit (core dumped)\n", STDERR_FILENO) != WRITE_SUCCESS)
					return (WRITE_ERR);
			}
			else if (xe->stat_loc == SIGINT)
			{
				if (ft_putstr_fd("\n", STDERR_FILENO) != WRITE_SUCCESS)
					return (WRITE_ERR);
			}
			xe->stat_loc = (xe->stat_loc % 256) + 128;
		}
		else if(WIFEXITED(xe->stat_loc))
			xe->stat_loc = WEXITSTATUS(xe->stat_loc);
		i++;
	}
	if (xe->flags & EXIT_FLAG || (ret != SUCCESS && ret < _ERRNO_MSG_))
		return (ret);
	else
	{
		xe->stat_loc = tmp;
		ft_error(ret, xe);
		if (ret == EXIT_ARG_ERR)
			return (SUCCESS);
		return (handle_execution(xe, STDIN_FILENO, 0));
	}
}

int			handle_command(t_command *cur_command, t_xe *xe, int fd_in, int proc)
{
	int			ret;

	ret = SUCCESS;
	if (cur_command->args == NULL)
	{
		free_command(cur_command);
		dup2(xe->backup_stdout, STDOUT_FILENO);
		dup2(xe->backup_stdin, STDIN_FILENO);
		return (handle_execution(xe, STDIN_FILENO, 0));
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

int			handle_execution(t_xe *xe, int fd_in, int proc)
{
	t_command	*cur_command;

	xe->gpid = -1;
	cur_command = parse_one_command(xe);
	if (cur_command == NULL)
		return (MALLOC_ERR);
	if (cur_command->redir_types != NULL)
	{
		if (cur_command->redir_types[0] == AMBIG)
		{
			free_command(cur_command);
			if (ft_error(AMBIG_REDIR, xe) != SUCCESS)
				return (FAILURE);
			dup2(xe->backup_stdout, STDOUT_FILENO);
			dup2(xe->backup_stdin, STDIN_FILENO);
			xe->stat_loc = 1;
			return (handle_execution(xe, fd_in, proc));
		}
		if (cur_command->args == NULL)
		{
			if (apply_redirs(cur_command->redir_paths, cur_command->redir_types, xe) != SUCCESS)
			{
				free_command(cur_command);
				return (FD_ERROR);
			}
			free_command(cur_command);
			return (handle_execution(xe, fd_in, proc));
		}
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

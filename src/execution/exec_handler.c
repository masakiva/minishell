#include "parsing.h"
#include "execution.h"

void		child_setup(const int *fd, int fd_in, t_xe *xe)
{
	xe->flags += CHILD;
	signal(SIGINT, SIG_DFL); // error if SIG_ERR? (check errno)
	signal(SIGQUIT, SIG_DFL); // error if SIG_ERR? (check errno)
	close(fd[0]);// error
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) != -1)
			close(fd_in);
	}
	if (dup2(fd[1], STDOUT_FILENO) != -1)// error
		close(fd[1]);
}

int			handle_pipe(t_command *cur_command, t_xe *xe, int fd_in, int proc)
{
	int			fd[2];
	int			ret;

	ret = SUCCESS;
	pipe(fd);// error?
	xe->gpid = fork();// error?
	if (xe->gpid == 0)
	{
		child_setup(fd, fd_in, xe);
		ret = execute_cmd(cur_command->args, cur_command->redir_paths, cur_command->redir_types, xe);// error
		free_command(cur_command);
		xe->flags -= RUN;
		return (ret);
	}
	else
	{
		if (!(xe->flags & CMD_PIPE))
			xe->flags += CMD_PIPE;
		close(fd[1]);// error
		close(fd_in);
		free_command(cur_command);
		return (handle_execution(xe, fd[0], proc + 1));
	}
}

int			parent_pipe_end(t_command *cur_command, t_xe *xe, int fd_in, int proc)
{
	int			i;
	int			ret;
	int			tmp;

	ret = SUCCESS;
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) != -1)
			close(fd_in);
	}
	dup2(xe->backup_stdout, STDOUT_FILENO);
	ret = execute_cmd(cur_command->args, cur_command->redir_paths, cur_command->redir_types, xe);// error
	tmp = xe->stat_loc;
	free_command(cur_command);
	dup2(xe->backup_stdout, STDOUT_FILENO);
	dup2(xe->backup_stdin, STDIN_FILENO);
	i = 0;
	while (i < proc)
	{
		wait(&xe->stat_loc);
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
	if ((cur_command->args) == NULL)
	{
		//ne peut être que le dernier argument car le parsing aurait renvoyé une erreur dans les autres cas
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

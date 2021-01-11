#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "pre_execution.h"
#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid

void		child_setup(const int *fd, int fd_in)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
		child_setup(fd, fd_in);
		ret = execute_cmd(cur_command->args, cur_command->redirs, xe);// error
		return (CLEAN_EXIT);
	}
	else
	{
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

	ret = SUCCESS;
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) != -1)
			close(fd_in);
	}
	ret = execute_cmd(cur_command->args, cur_command->redirs, xe);// error
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
		return (SUCCESS);
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
	cur_command = NULL;
	if (xe->commands != NULL)
		cur_command = ft_lstshift(&(xe->commands));
	if (cur_command != NULL) // le contraire possible?
	{
		return (handle_command(cur_command, xe, fd_in, proc));
	}
	else
	{
		dup2(xe->backup_stdout, STDOUT_FILENO);
		dup2(xe->backup_stdin, STDIN_FILENO);
		return (SUCCESS);
	}
}

#include "execution.h"
#include "parsing.h"

int		parsing_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified",
		"Empty command before ; or |"};

	ft_putstr_fd("syntax error: ", STDERR_FILENO);
	ft_putendl_fd(err_msg[err_code - _PARSING_ERROR_ - 1], STDERR_FILENO);
	xe->stat_loc = 2;
	return (SUCCESS);
}

int		exec_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"Ambiguous redirection",
		"No such file or directory", // more specific msg
		"export: Variable identifier (name) invalid",
		"unset: Variable identifier (name) invalid",
		"HOME not set",
		"cd: too many arguments",
		"exit: too many arguments"};

	ft_putstr_fd("command error: ", STDERR_FILENO);
	ft_putendl_fd(err_msg[err_code - _EXEC_ERROR_ - 1], STDERR_FILENO);
	if (err_code == NO_SUCH_FILE)
		xe->stat_loc = 127;
	if (xe->flags & EXEC_PIPE || xe->flags & CHILD)
		xe->flags = 0;
	else if (err_code == EXIT_ARG_ERR)
		xe->flags = RUN;
	return (SUCCESS);
}

static int			err_output(int err_code, t_xe *xe)
{
	const char	*err_msg[] = {
		"Memory allocation failure",
		"Cannot write on standard output",
		"Cannot read standard input (GNL error)",
		"cd: cannot change directory",
		"cd: cannot set variable PWD",
		"pwd: cannot get current directory path",
		"Error reading a directory in PATH",
		"fd",
		"external command error"}; // fd?

	(void)xe;
	ft_putstr_fd("error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code - _ERRNO_MSG_ - 1], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO); // besoin de faire comme perror?
	ft_putendl_fd(strerror(errno), STDERR_FILENO); // strerror error?
	if (err_code == EXT_CMD_ERROR)
	{
		if (errno == ENOENT)
			xe->stat_loc = 127;
		else if (errno == EACCES || errno == ENOEXEC)
			xe->stat_loc = 126;
		exit(xe->stat_loc);
	}
	return (SUCCESS);
}

int				clean_and_exit(int err_code, t_xe *xe)
{
	int	ret;

	if (err_code == ARG_ERR)
	{
		ret = 1;
		ft_putstr_fd("minishell takes no argument\n", STDERR_FILENO);
	}
	else if (err_code == SUCCESS)
		ret = EXIT_SUCCESS;
	else
	{
		if (err_code == EXT_CMD_ERROR)
		{
			ft_putstr_fd("External function error: ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		}
		else if (err_code == EXIT_NAN)
			ft_putstr_fd("exit: bad argument\n", STDERR_FILENO);
		ret = (xe->stat_loc);
	}
	if (!(xe->flags & EXIT_ABORT))
	{
		free_str_array(xe->exported); // besoin de free?
		free_str_array(xe->env); // besoin de free?
		free(xe);
		exit(ret);
	}
	else
	{
		xe->flags = RUN;
		return (SUCCESS);
	}
}

int					ft_error(int ret, t_xe *xe)
{
	if (xe->flags & EXIT_FLAG && !(xe->flags & CHILD))
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (ret > _ERRNO_MSG_)
		ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (ret > _PARSING_ERROR_)
		return (parsing_error(ret, xe));
	else if (ret > _EXEC_ERROR_)
		return (exec_error(ret, xe));
	else if (ret > _ERRNO_MSG_)
		err_output(ret, xe);
	else if (ret > _EXIT_CODE_ || !(xe->flags & RUN) || (xe->flags & EXIT_FLAG))
		return (clean_and_exit(ret, xe));
	else if (ret != SUCCESS)
		ft_putstr_fd("Fatal error", STDERR_FILENO);
	return (SUCCESS);
}

int		error_and_exit(enum e_retcode ret, t_xe *xe)
{
	ft_error(ret, xe);
	return (clean_and_exit(ret, xe));
}

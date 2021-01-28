#include "minishell.h"
#include "execution.h"
#include "parsing.h"
#include <stdio.h> // perror

void		putstr_stderr(const char *str)
{
	if (ft_putstr_fd(str, STDERR_FILENO) == ERROR)
	{
		perror("Cannot write the error message on STDERR");
		exit(EXIT_FAILURE);
	}
}

int		parsing_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified",
		"Empty command before ; or |"};

	(void)err_msg;
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code - _PARSING_ERROR_ - 1], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	xe->stat_loc = 2;
	return (SUCCESS);
}


int		exec_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"Ambiguous redirection",
		"HOME not set",
		"No such file or directory"};

	(void)err_msg;
	ft_putstr_fd("minishell: command error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code - _EXEC_ERROR_ - 1], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	// have to set stat_loc as well !!!
	if (err_code == HOME_NOT_SET)
		xe->stat_loc = 1;
	if (err_code == NO_SUCH_FILE)
		xe->stat_loc = 127;
	if (xe->child == 1)
		return (CHILD_EXIT);
	else
		return (SUCCESS);
}

static const char	*err_msg(int err_code)
{
	const char	*msg[] = {
		"Memory allocation failure",
		"Cannot write on standard output",
		"Cannot read standard input (GNL error)"};

	return (msg[err_code]);
}

static int			err_output(int err_code)
{
	putstr_stderr("Error: ");
	perror(err_msg(err_code - _ERRNO_MSG_));
	return (SUCCESS);
}

int				clean_and_exit(int ret, t_xe *xe)
{
	free_str_array(xe->exported); // besoin de free?
	free_str_array(xe->env); // besoin de free?
	free(xe);
	if (ret == ARG_ERR)
		putstr_stderr("Minishell takes no argument");
	if (ret != CHILD_EXIT)
	if (isatty(STDIN_FILENO)) // temp pour le testeur
		write(1, "exit\n", 5);
	exit(EXIT_SUCCESS);
}

int					ft_error(int ret, t_xe *xe)
{
	if (ret > _PARSING_ERROR_)// temp
		return (parsing_error(ret, xe));
	else if (ret > _EXEC_ERROR_)
		return (exec_error(ret, xe));
	else if (ret > _ERRNO_MSG_)
		err_output(ret);
	else if (ret > _EXIT_CODE_)
		return (clean_and_exit(ret, xe));
	else if (ret != SUCCESS) // temp
		putstr_stderr("ERROR CODE ERROR (printed for debug)");// temp
	return (SUCCESS);
}

int					ft_exit(enum e_retcode ret, t_xe *xe)
{
	ft_error(ret, xe);
	return (clean_and_exit(ret, xe));
}

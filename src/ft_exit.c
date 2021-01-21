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
	const char		*err_msg[NB_PARSING_ERRORS] = {
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified",
		"Empty command before ; or |"};

	(void)err_msg;
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	xe->stat_loc = 2;
	return (SUCCESS);
}


int		exec_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"HOME not set",
		"No such file or directory"};

	(void)err_msg;
	ft_putstr_fd("minishell: command error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code - 7], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	// have to set stat_loc as well !!!
	if (err_code == HOME_NOT_SET)
		xe->stat_loc = 1;
	if (err_code == NO_SUCH_FILE)
		xe->stat_loc = 127;
	if (xe->child == 1)
		return (CLEAN_EXIT);
	else
		return (SUCCESS);
}

static const char	*err_msg(int err_code)
{
	const char	*msg[] = {
		"Minishell takes no argument",
		"Memory allocation failure",
		"Cannot write on standard output",
		"Cannot read standard input (GNL error)"};

	return (msg[err_code]);
}

static int			err_output(int err_code)
{
	putstr_stderr("Error\n");
	if (err_code == ARG_ERR)
	{
		putstr_stderr(err_msg(err_code - 3));
		putstr_stderr("\n");
	}
	else if (err_code < HOME_NOT_SET)
		perror(err_msg(err_code - 3));
	write(1, "exit\n", 7);
	exit(EXIT_FAILURE);
}

int					ft_exit(enum e_retcode ret, t_xe *xe)
{
	if (ret >= SQUOTE_MISSING)// temp
		return (parsing_error(ret - 9, xe));
	else if (ret >= HOME_NOT_SET)
		return (exec_error(ret - 7, xe));
	else if (ret > CLEAN_EXIT)
		err_output(ret);
	else
		putstr_stderr("ERROR CODE ERROR");// temp
	free_str_array(xe->exported); // besoin de free?
	free_str_array(xe->env); // besoin de free?
	free(xe);
	write(1, "exit\n", 5);
	exit(EXIT_SUCCESS);
}

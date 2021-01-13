#include "execution.h"

int		exec_error(int err_code)
{
	const char		*err_msg[] = {
		"HOME not set",
		"No such file or directory"};

	(void)err_msg;
	ft_putstr_fd("minishell: command error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	// have to set stat_loc as well !!!
	return (SUCCESS);
}

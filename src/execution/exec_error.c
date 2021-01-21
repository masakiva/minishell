#include "execution.h"

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
	return (SUCCESS);
}

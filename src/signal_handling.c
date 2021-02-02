#include "minishell.h"

void	sig_int(int signum)
{
	(void)signum;
//	ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
		return ;
}

void	sig_kill(int signum)
{
	(void)signum;
	ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
//	ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
//	ft_putstr_fd("\n", STDOUT_FILENO);
//	if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
//		return ;
}

void				signal_handler(void)
{
	signal(SIGINT, sig_int); // error if SIG_ERR? (check errno)
	signal(SIGQUIT, sig_kill); // error if SIG_ERR? (check errno)
}

#include "signals.h"

void	sig_int(int signum)
{
	(void)signum;
	//	ft_putstr_fd("SIGINT", 1);
	ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
		return ;
	//	kill(0, signum);
//	exit(signum);
	//	signal(signum, SIG_DFL);
}

void	sig_kill(int signum)
{
	(void)signum;
	//	ft_putstr_fd("SIGQUIT", 1);
	ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
		return ;
	//	kill(0, signum);
//	exit(signum);
	//	signal(signum, SIG_DFL);
}

void				signal_handler(void)
{
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_kill);
}

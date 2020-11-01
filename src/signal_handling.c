#include "signals.h"

void	sig_int(int signum)
{
	//	ft_putstr_fd("SIGINT", 1);
	ft_putstr_fd("\n", STDOUT_FILENO);
	//	kill(0, signum);
	exit(signum);
	//	signal(signum, SIG_DFL);
}

void	sig_kill(int signum)
{
	//	ft_putstr_fd("SIGQUIT", 1);
	ft_putstr_fd("\n", STDOUT_FILENO);
	//	kill(0, signum);
	exit(signum);
	//	signal(signum, SIG_DFL);
}

void				signal_handler(void)
{
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_kill);
}

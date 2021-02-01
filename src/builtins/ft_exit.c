#include "builtins.h"

int		ft_exit(enum e_retcode ret, t_xe *xe)
{
	ft_error(ret, xe);
	return (clean_and_exit(ret, xe));
}

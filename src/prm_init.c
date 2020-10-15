#include <unistd.h>
#include "minishell.h"

static int	extract_env(t_all *all)
{
	int		i;
	char	*buff;

	i = 0;
	all->env = NULL;
	if (all->current == NULL)
		return (-1);
	while ((all->current)[i])
	{
		if ((buff = ft_strdup((all->current)[i])) == NULL)
			return (MALLOC_ERR);
		ft_lstadd_back(&all->env, ft_lstnew(buff));
		i++;
	}
	all->current = NULL;
	return (SUCCESS);
}

int			all_init(t_all *all)
{
	all->fd[0] = STDIN_FILENO;
	all->fd[1] = STDOUT_FILENO;
	all->fd[2] = STDERR_FILENO;
	return (extract_env(all));
}

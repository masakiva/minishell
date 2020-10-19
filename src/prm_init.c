#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static int	extract_env(t_all *all)
{
	int		i;

	if (!(all->env = malloc((ft_arraylen(all->current) + 1) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	while (all->current[i])
	{
		all->env[i] = ft_strdup(all->current[i]);
		i++;
	}
	all->env[i] = NULL;
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

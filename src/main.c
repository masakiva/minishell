#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid
#include <fcntl.h> // open

static int			handle_eof(char **line, int ret)
{
	char	*tmp;

	while (ret != SUCCESS)
	{
		ft_putstr_fd("  \b\b", STDIN_FILENO);
		ret = get_next_line(STDIN_FILENO, &tmp);
		*line = ft_strjoin(*line, tmp);
	}
	return (ret);
}

static int			get_input(char **line)
{
	int		ret;

	if (isatty(STDIN_FILENO)) // temp pour le testeur
	{
		if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) != WRITE_SUCCESS)
			return (WRITE_ERR);
	}
	ret = get_next_line(STDIN_FILENO, line);
	if (ret == ERROR)
		return (GNL_ERR);
	else if (ret == 0) // EOF in files and heredocs (noeol files not yet supported)
	{
		if (ft_strcmp(*line, "") == 0)
		{
			free(*line);
			return (CLEAN_EXIT);
		}
		else
			return (handle_eof(line, ret));
	}
	return (SUCCESS);
}

static int			main_loop(t_xe *xe)
{
	int			ret;
	char		*line;

	ret = get_input(&line);
	if (ret == SUCCESS)
	{
		xe->line = line;
		ret = check_syntax(line);
		if (ret == SUCCESS)
			ret = handle_execution(xe, STDIN_FILENO, 0);
		free(line);
		ret = ft_error(ret, xe);
	}
	return (ret);
}

int		exec_env_init(t_xe *xe, char **env_source)
{
	char	*shlvl;
	int		tmp;

	xe->env = dup_str_array(env_source);
	if (xe->env == NULL)
		return (MALLOC_ERR);
	shlvl = get_var_value(xe->env, SHLVL_STR, ft_strlen(SHLVL_STR));
	if (shlvl == NULL)
		return (MALLOC_ERR);
	tmp = ft_atoi(shlvl);
	free(shlvl);
	tmp += 1;
	shlvl = ft_itoa(tmp);
	if (shlvl == NULL)
		return (MALLOC_ERR);
	env_replace_var(SHLVL_STR, shlvl, xe);
	free(shlvl);
	xe->backup_stdin = dup(STDIN_FILENO);
	xe->backup_stdout = dup(STDOUT_FILENO);
	return (SUCCESS);
}

int		read_from_file(t_xe *xe, char *file)
{
	int	fd;
	int	flags;

	flags = O_RDONLY;
	fd = open(file, flags);
	if (fd == ERROR)
		return (-1); // another error code
	dup2(fd, STDIN_FILENO);
	xe->backup_stdin = dup(STDIN_FILENO);
	return (SUCCESS);
}

int		main(int argc, char **argv, char **env_source)
{
	int		ret;
	t_xe	*xe;

	(void)argv;
	ret = SUCCESS;
	xe = (t_xe *)malloc(sizeof(t_xe));
	if (xe == NULL)
		return (ft_exit(MALLOC_ERR, xe));
	ft_bzero(xe, sizeof(t_xe));
	if (argc > 2)
		ret = ARG_ERR;
	else if (argc == 2)
	{
		//signal_handler(); ?
		ret = exec_env_init(xe, env_source);
		ret = read_from_file(xe, argv[1]);
		if (ret != SUCCESS) // needed?
			return (ft_exit(ret, xe)); // needed?
		while (ret == SUCCESS)
			ret = main_loop(xe);
	}
	else // merge with else if (argc == 2) ?
	{
		signal_handler(); // err?
		ret = exec_env_init(xe, env_source);
		if (ret != SUCCESS) // needed?
			return (ft_exit(ret, xe)); // needed?
		while (ret != CLEAN_EXIT && ret != FAILURE && ret != CHILD_EXIT) // why not while (ret == SUCCESS) ?
			ret = main_loop(xe);
	}
	return (ft_exit(ret, xe));
}

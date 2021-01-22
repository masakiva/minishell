#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "pre_execution.h"
#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid
#include <fcntl.h>

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
		if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
			return (WRITE_ERR);
	}
	ret = get_next_line(STDIN_FILENO, line);
	if (ret == ERROR)
		return (GNL_ERR);
	else if (ret == 0) // EOF in files and heredocs (noeol files not yet supported)
	{
		if (ft_strcmp(*line, "") == 0)
			return (CLEAN_EXIT);
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
	xe->line = line;
	if (ret == SUCCESS)
	{
		if ((ret = check_syntax(line)) != SUCCESS)
			return (ft_error(ret, xe));
		ret = handle_execution(xe, STDIN_FILENO, 0);
		//printf("ret = %d\n", ret);
		free(line);
		if (ret != SUCCESS)
			return (ft_error(ret, xe));
	}
	return (ret);
}

int		exec_env_init(t_xe *xe, char **env_source)
{
	char	*shlvl;
	char	*val;
	int		tmp;
	int		ret;

	xe->env = dup_str_array(env_source);
	xe->child = 0;
	if (xe->env == NULL)
		ret = MALLOC_ERR;
	shlvl = get_var_value(xe->env, SHLVL_STR, ft_strlen(SHLVL_STR));
//	printf("shlvl = %s\n", shlvl);
	tmp = ft_atoi(shlvl);
	tmp += 1;
	val = ft_itoa(tmp);
	env_replace_var(SHLVL_STR, val, xe);
	xe->backup_stdin = dup(STDIN_FILENO);
	xe->backup_stdout = dup(STDOUT_FILENO);
	return (SUCCESS);
}

int		read_from_file(t_xe *xe, char **argv)
{
	int	fd;
	int	flags;

	flags = O_RDONLY;
	fd = open(argv[1], flags);
	if (fd < 0)
		return (-1);
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
	if (xe == NULL)
		ret = MALLOC_ERR;
	else if (argc > 2)
		ret = ARG_ERR;
	else if (argc == 2)
	{
		ret = exec_env_init(xe, env_source);
		ret = read_from_file(xe, argv);
		if (ret != SUCCESS)
			return (ft_exit(ret, xe));
		while (ret == SUCCESS)
			ret = main_loop(xe);
	}
	else
	{
		signal_handler(); // err?
		ret = exec_env_init(xe, env_source);
		if (ret != SUCCESS)
			return (ft_exit(ret, xe));
		while (ret != CLEAN_EXIT && ret != FAILURE)
			ret = main_loop(xe);
	}
	return (ft_exit(ret, xe));
}

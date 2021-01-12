#include "minishell.h"
#include "signals.h"
#include "parsing.h"
#include "pre_execution.h"
#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid

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
		free(*line);
		return (CLEAN_EXIT);
	}
	return (ret);
}

static int			main_loop(t_xe *xe)
{
	int			ret;
	char		*line;

	ret = get_input(&line);
	xe->line = line;
	if (ret == SUCCESS)
	{
		//check_syntax(line);
		ret = handle_execution(xe, STDIN_FILENO, 0);
		free(line);
	}
	return (ret);
}

int		exec_env_init(t_xe *xe, char **env_source)
{
	char		*shlvl;
	char		*val;
	int			tmp;
	int		ret;

	xe->env = dup_str_array(env_source);
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

int		main(int argc, char **argv, char **env_source)
{
	int		ret;
	t_xe 	*xe;

	(void)argv;
	ret = SUCCESS;
	xe = (t_xe *)malloc(sizeof(t_xe));
	if (xe == NULL)
		return (ft_exit(MALLOC_ERR, xe));
	ft_bzero(xe, sizeof(t_xe));
	if (xe == NULL)
		ret = MALLOC_ERR;
	else if (argc != 1)
		ret = ARG_ERR;
	else
	{
	//	signal_handler(); // err?
		ret = exec_env_init(xe, env_source);
		if (ret != SUCCESS)
			return (ft_exit(ret, xe));
		while (ret == SUCCESS)
			ret = main_loop(xe);
	}
	return (ft_exit(ret, xe));
}

#include "execution.h"
#include "parsing.h"

int		parsing_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified",
		"Empty command before ; or |"};

	ft_putstr_fd("syntax error: ", STDERR_FILENO);
	ft_putendl_fd(err_msg[err_code - _PARSING_ERROR_ - 1], STDERR_FILENO);
	xe->stat_loc = 2;
	return (SUCCESS);
}


int		exec_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"Ambiguous redirection",
		"No such file or directory", // more specific msg
		"export: Variable identifier (name) invalid",
		"unset: Variable identifier (name) invalid",
		"HOME not set",
		"cd: too many arguments",
		//"pwd : erreur de détermination du répertoire actuel : getcwd : ne peut accéder aux répertoires parents : Aucun fichier ou dossier de ce type",// -> moved to err_output
		"exit: too many arguments"};


	ft_putstr_fd("command error: ", STDERR_FILENO);
	ft_putendl_fd(err_msg[err_code - _EXEC_ERROR_ - 1], STDERR_FILENO);
	// have to set stat_loc as well !!!
	if (err_code == HOME_NOT_SET)
		xe->stat_loc = 1;
	if (err_code == NO_SUCH_FILE)
		xe->stat_loc = 127;
	if (xe->pipe & EXEC_PIPE)
		return (CHILD_EXIT);
	else
		return (SUCCESS);
}

static int			err_output(int err_code)
{
	const char	*err_msg[] = {
		"Memory allocation failure",
		"Cannot write on standard output",
		"Cannot read standard input (GNL error)",
		"cd: cannot change directory",
		"cd: cannot set variable PWD",
		"pwd: cannot get current directory path",
		"Error reading a directory in PATH",
		"fd"}; // fd?

	ft_putstr_fd("error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code - _ERRNO_MSG_ -1], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO); // besoin de faire comme perror?
	ft_putendl_fd(strerror(errno), STDERR_FILENO); // strerror error?
	return (SUCCESS);
}

int				clean_and_exit(int err_code, t_xe *xe)
{
	int	ret;

	if (err_code == ARG_ERR)
	{
		if (ft_putstr_fd("Minishell takes no argument", STDERR_FILENO) != WRITE_SUCCESS) // takes no argument?
			ft_error(WRITE_ERR, xe); // possible?
	}
	if (err_code < FT_EXIT)
		ret = EXIT_SUCCESS;
	else
	{
		if (err_code == CHILD_ERROR)
		{
			ft_putstr_fd("External function error: ", STDERR_FILENO); // -> strerror
			ft_putendl_fd(strerror(errno), STDERR_FILENO); // strerror error?
		}
		else if (err_code == EXIT_NAN)
			ft_putstr_fd("exit: bad argument\n", STDERR_FILENO); // -> strerror
		ret = (xe->stat_loc);
	}
	free_str_array(xe->exported); // besoin de free?
	free_str_array(xe->env); // besoin de free?
	free(xe);
	if (isatty(STDIN_FILENO)) // temp pour le testeur
	{
		if (err_code == CLEAN_EXIT || err_code == FT_EXIT)
		{
			if (ft_putstr_fd("exit\n", STDOUT_FILENO) != WRITE_SUCCESS)
				return (WRITE_ERR); // possible?
		}
	}
	exit(ret);
}

int					ft_error(int ret, t_xe *xe)
{
	if (ret == PIPE_EXIT)
	{
		xe->pipe = 0;
		return (SUCCESS);
	}
	if (ret > CHILD_EXIT)
		ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (ret > _PARSING_ERROR_)// temp
		return (parsing_error(ret, xe));
	else if (ret > _EXEC_ERROR_)
		return (exec_error(ret, xe));
	else if (ret > _ERRNO_MSG_)
		err_output(ret);
	else if (ret > _EXIT_CODE_)
		return (clean_and_exit(ret, xe));
	else if (ret != SUCCESS) // temp
		ft_putstr_fd("ERROR CODE ERROR (printed for debug)", STDERR_FILENO);// temp
	return (SUCCESS);
}

int		error_and_exit(enum e_retcode ret, t_xe *xe)
{
	ft_error(ret, xe);
	return (clean_and_exit(ret, xe));
}

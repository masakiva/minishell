#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "signals.h"

# define ECHO_OPTIONS	"n"
# define ECHO_N_OPTION	0b00000001

enum		e_cmd_code
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	ELSE,
	M_ERROR
};

typedef int			(*t_exec)(char **args, t_xe *xe);

pid_t	gpid;
int		fd_pipe[2];
int		stat_loc;
int		fd_old;
int		fd_backup;

/*
**	main functions
*/

char	**prepare_args(t_command *command, char **env);
int		execute_cmd(char **args, t_xe *xe);

/*
**	builtins
*/

int		ft_echo(char **args, t_xe *xe);
int		ft_pwd(char **args, t_xe *xe);
int		ft_cd(char **args, t_xe *xe);
int		ft_export(char **args, t_xe *xe);
int		ft_unset(char **args, t_xe *xe);
int		ft_env(char **args, t_xe *xe);

/*
**	utils
*/

t_byte		pop_options(char ***args, char *op_chars);

#endif

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "signals.h"

# define ECHO_OPTIONS	"n"
# define ECHO_N_OPTION	0b00000001

typedef int			(*t_exec)(char **args, char **env);

int		stat_loc;
pid_t	gpid;

/*
**	main functions
*/

char	**prepare_args(t_command *command, char **env);
int		execute_cmd(char **args, char **env);

/*
**	utils
*/

enum	e_cmd_code	get_cmd_code(char *arg);

#endif

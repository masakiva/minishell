#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "signals.h"

# define ECHO_OPTIONS	"n"
# define ECHO_N_OPTION	0b00000001

# define ENTIRE_STR	-1
# define NOT_FOUND	-1

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
	M_ERROR // another way?
};

typedef int			(*t_exec)(char **args, t_xe *xe);

/*
**	main function
*/

int		execute_cmd(char **args, char **redir_paths, enum e_redir_op *redir_types, t_xe *xe);

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

t_byte		shift_options(char ***args, char *op_chars);
int		env_replace_var(char *var_name, char *value, t_xe *xe);
int			check_var_name(char *var, ssize_t name_len);

#endif

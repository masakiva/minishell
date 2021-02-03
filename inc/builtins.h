#ifndef BUILTINS_H
# define BUILTINS_H

# include "execution.h"
# include "error.h"

# define ECHO_OPTIONS	"n"
# define ECHO_N_OPTION	0b00000001

# define ENTIRE_STR	-1

/*
**	builtins
*/

int		ft_echo(char **args, t_xe *xe);
int		ft_pwd(char **args, t_xe *xe);
int		ft_cd(char **args, t_xe *xe);
int		ft_export(char **args, t_xe *xe);
int		ft_unset(char **args, t_xe *xe);
int		ft_env(char **args, t_xe *xe);
int		ft_exit(char **args, t_xe *xe);

/*
**	utils
*/

t_byte		shift_options(char ***args, char *op_chars);
int			check_var_name(char *var, ssize_t name_len);

#endif

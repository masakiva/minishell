#ifndef EXECUTION_H
# define EXECUTION_H

#include <sys/types.h> // waitpid, opendir
#include <sys/wait.h> // waitpid
#include <fcntl.h> // open
#include <dirent.h> // readdir, opendir

# include "minishell.h"

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

void	apply_redirs(char **redir_paths, enum e_redir_op *redir_types);

#endif

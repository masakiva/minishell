#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h> // temp, for printf

/*
**	**********************************
**	**		GENERAL INCLUDES		**
**	**********************************
*/

# include <unistd.h>
# include <stdlib.h>
# include <errno.h>

# include "libft.h"


/*
**	**********************************
**	**		GENERAL DEFINES			**
**	**********************************
*/

# define SUCCESS	1
# define FAILURE	0

# define TRUE		1
# define FALSE		0

# define _XOPEN_SOURCE // qu'est-ce?

# define FT_PS1			"$ "

# define BUILTINS		"echo/cd/pwd/export/unset/env/exit"

# define SHLVL_STR		"SHLVL"

# define TO_SPLIT		1
# define NOT_TO_SPLIT	0


/*
**	**********************************
**	**	GENERAL TYPES AND STRUCTS	**
**	**********************************
*/

enum		e_retcode
{
	CLEAN_EXIT = 2,
	ARG_ERR,
	MALLOC_ERR,
	WRITE_ERR,
	GNL_ERR,
};

enum e_redir_op
{
	NO_REDIR,
	FILEIN,
	FILEOUT,
	APPEND
};

typedef struct		s_command
{
	char				**args;
	char				**redir_paths;
	enum e_redir_op		*redir_types;
	t_byte				pipe_flag;
	uint8_t				pad[7];
}					t_command;

typedef struct		s_xe
{
	pid_t	gpid;
	int		child;
	int		backup_stdin;
	int		backup_stdout;
	int		stat_loc;
	char	*line;
	char	**env;
	char	**exported;
}					t_xe;


/*
**	**********************************
**	**			PROTOTYPES			**
**	**********************************
*/

/*
**	free utils
*/
int			handle_execution(t_xe *xe, int fd_in, int proc);

/*
**	free utils
*/

void	print_tokens(t_list *tokens); // temp
void	free_token(void *content);
void	free_command(void *content);
void	free_commands(t_list **commands);

/*
**	ft_exit.c
*/

void		putstr_stderr(const char *str);
int			ft_error(int ret, t_xe *xe);
int			ft_exit(enum e_retcode retcode, t_xe *xe);

#endif

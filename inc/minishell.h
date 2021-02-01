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
# include <signal.h>

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
	_EXIT_CODE_ = 2,
	CLEAN_EXIT,
	CHILD_EXIT,
	ARG_ERR,
	_ERRNO_MSG_,
	MALLOC_ERR,
	WRITE_ERR,
	GNL_ERR,
	_EXEC_ERROR_,
	AMBIG_REDIR,
	HOME_NOT_SET,
	NO_SUCH_FILE,
	INVALID_EXP_ID,
	INVALID_UNS_ID,
	INVALID_PWD_PATH,
	_PARSING_ERROR_,
	SQUOTE_MISSING,
	DQUOTE_MISSING,
	ESCAPE_NL,
	REDIR_PATH_INVALID,
	REDIR_PATH_MISSING,
	EMPTY_CMD
};

enum	e_redir_op
{
	NO_REDIR,
	FILEIN,
	FILEOUT,
	APPEND,
	AMBIG
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
	uint8_t	pad[4];
	char	*line;
	char	**env;
	char	**exported;
}					t_xe;


/*
**	**********************************
**	**			PROTOTYPES			**
**	**********************************
*/

void		signal_handler(void);

int			handle_execution(t_xe *xe, int fd_in, int proc);

int			env_replace_var(char *var_name, char *value, t_xe *xe);

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

int			clean_and_exit(int ret, t_xe *xe);
void		putstr_stderr(const char *str);
int			ft_error(int ret, t_xe *xe);

#endif

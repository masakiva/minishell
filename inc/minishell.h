#ifndef MINISHELL_H
# define MINISHELL_H

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
	PARSING_ERR = 2,
	CLEAN_EXIT,
	ARG_ERR,
	MALLOC_ERR,
	WRITE_ERR,
	GNL_ERR
};

enum e_redir_op
{
	NO_REDIR,
	FILEIN,
	FILEOUT,
	APPEND
};

typedef struct		s_var_props
{
	size_t	start;
	size_t	len;
	t_byte	split_flag;
	uint8_t	pad[7];
}					t_var_props;

typedef struct		s_token
{
	char			*str;
	t_list			*var_properties;
	enum e_redir_op	redir;
	uint8_t			pad[4];
}					t_token;

typedef struct		s_command
{
	t_list		*tokens;
	t_byte		pipe_flag;
	uint8_t		pad[7];
}					t_command;

typedef struct		s_xe
{
	pid_t	gpid;
	int		backup_stdin;
	int		backup_stdout;
	int		stat_loc;
	char	**env;
	char	**exported;
	t_list	*commands;
}					t_xe;


/*
**	**********************************
**	**			PROTOTYPES			**
**	**********************************
*/

/*
**	free utils
*/

void		print_tokens(t_list *tokens); // temp
void	free_token(void *content);
void	free_command(void *content);
void	free_commands(t_list **commands);

/*
**	ft_exit.c
*/

void		putstr_stderr(const char *str);
int			ft_exit(enum e_retcode retcode, t_xe *xe);

#endif

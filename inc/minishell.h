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

# define PROMPT			"$ "

# define BUILTINS		"echo/cd/pwd/export/unset/env/exit"

/*
**
**	SHELL METACHARS AND OPERATORS
**
**	# define METACHARS		"|&;()<> \t\n"
**	# define CTRL_OPS		"||/&&/&/;/;;/;&/;;&/|/|&/(/)"
**	# define REDIR_OPS		"</>/>|/>>/&>/>&/<<"
**	# define DQUOTES_ESC	"$`\"\\\n"
*/


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
	GNL_ERR
};

enum e_redir_op
{
	NO_REDIR,
	FILEIN,
	FILEOUT,
	APPEND
};

typedef struct		s_var_pos
{
	size_t	start;
	size_t	len;
}					t_var_pos;

typedef struct		s_token
{
	char			*str;
	t_list			*var_positions;
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
	int		fd[3]; // a priori inutile
	int		stat_loc;
	char	**env;
	char	**exported;
	t_list	*commands;
	pid_t	gpid;	// nécessaire ?
	uint8_t	pad[4];
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

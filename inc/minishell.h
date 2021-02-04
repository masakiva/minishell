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

# define SHLVL_STR		"SHLVL"

# define TO_SPLIT		1
# define NOT_TO_SPLIT	0

# define EXEC_PIPE		0x01
# define CMD_PIPE		0x02
# define CHILD_EXIT		0x04
# define CHILD_ERROR	0x08

/*
**	**********************************
**	**	GENERAL TYPES AND STRUCTS	**
**	**********************************
*/

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
	t_byte				var_flag;
	uint8_t				pad[6];
}					t_command;

typedef struct		s_xe
{
	pid_t	gpid;
	int		backup_stdin;
	int		backup_stdout;
	int		stat_loc;
	char	*line;
	char	**env;
	char	**exported;
	char	flags;
	uint8_t	pad[7];
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

#endif

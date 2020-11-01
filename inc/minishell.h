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

enum		e_errors
{
	CLEAN_EXIT = 2,
	ARG_ERR,
	MALLOC_ERR,
};

enum e_redir_op
{
	NO_REDIR,
	FILEIN,
	FILEOUT,
	APPEND
};

typedef struct		s_variable
{
	size_t	start;
	size_t	len;
	size_t	end; // a retirer
}					t_variable;

typedef struct		s_token
{
	char			*str;
	t_list			*vars;
	enum e_redir_op	redir;
	uint8_t			pad[4];
}					t_token;

typedef struct		s_command
{
	t_list		*tokens;
	t_byte		pipe_flag;
	uint8_t		pad[7];
}					t_command;


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

int					ft_exit(int err_code, char **env);












/*
**	str_utils.c
*/

//int					ft_strcmp(const char *s1, const char *s2);
//int					ft_isset(char c, char *set);
//char				*get_var_content(char **env, char *name);
//ssize_t				get_var_pos(char **env, char *name);
//void				ft_printarray_fd(char **array, int fd);
//size_t				ft_arraylen(char **array);
//void				free_str_array(char ***array);

/*
**	rec_gnl.c
*/
//int					rec_gnl(int fd, char **line);

/*
**	ft_split.c
**	char				**ft_split(char const *s, char set);
*/

typedef struct		s_all
{
	int				fd[3]; // a laisser ici
	int				cmd_ret;
	char			**env;
	t_list			*commands;
	char			**current; // a retirer
}					t_all;

/*
**	all_init.c
*/
int					all_init(t_all *all);


//typedef struct		s_exe
//{
//	t_cmd_code		cmd_code;
//	uint8_t			pad[4];
//	char			**args;
//	char			**env;
//}					t_exe;

#endif

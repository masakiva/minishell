#ifndef MINISHELL_H
# define MINISHELL_H

/*
**	# include <stdio.h>
**	# include <fcntl.h>
**	# include <errno.h>
**	# include <stdlib.h>
**	# include <string.h>
**	# include <sys/types.h>
**	# include <sys/wait.h>
*/

# include <errno.h>
# include "libft.h"

/*
**	# define _XOPEN_SOURCE
*/

/*
**	**********************************
**	**			DEFINES				**
**	**********************************
*/

# define CLEAN_EXIT		42// or 1 ?
# define SUCCESS		0 // or 1 ?
// # define ERROR	-1 dans libft.h
# define ARG_ERR		1
# define MALLOC_ERR		2

# define ISSPACE_3		"\f\n\r\t\v "

# define BUILTINS		"echo/cd/pwd/export/unset/env/exit"

# define PROMPT			"$ "

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
**	**	CUSTOM TYPES AND STRUCTURES	**
**	**********************************
*/

#include <stdio.h>

enum e_state
{
	LETTER,
	QUOTE,
	BACKSLASH,
	DOLLAR,
	SPACE,
	ANGLE_BRACKET,
	SEMICOLON,
	PIPE,
	//METACHAR,
	ERR,
	END,
	NB_STATES
};

enum e_parsing_error
{
	SQUOTE_MISSING,
	DQUOTE_MISSING,
	ESCAPE_NL,
	REDIR_PATH_INVALID,
	REDIR_PATH_MISSING,
	NB_PARSING_ERRORS
};

enum e_redir
{
	NO_REDIR,
	FILEIN,
	FILEOUT,
	APPEND
};

typedef struct		s_variable
{
	size_t	start;
	size_t	end;
}					t_variable;

typedef struct		s_token
{
	char			*str;
	t_list			*vars;
	enum e_redir	redir;
	uint8_t			pad[4];
}					t_token;

# define	TRUE	1
# define	FALSE	0
typedef struct		s_command
{
	t_list	*tokens;
	t_byte	pipe_flag;
	uint8_t	pad[7];
}					t_command;

# define BUF_SIZE	16

typedef struct		s_state_machine
{
	enum e_state			state;
	enum e_parsing_error	error;
	char					buf[BUF_SIZE];
	size_t					len;
	t_token					*cur_token;
}							t_state_machine;

void		print_tokens(t_list *tokens); // temp
void	free_token(void *content);
void	free_commands(t_list **commands);
int		new_command(t_list **commands);
int		add_variable(t_list **variables, size_t start, size_t end);
char	*parse_variable(char *line, t_state_machine *machine);
int		link_token(t_list **tokens, t_state_machine *machine);
int		reset_buf(t_state_machine *machine);
int		add_to_buf(char c, t_state_machine *machine);

t_list	*parse_input(char *line);


int		parse_commands(t_list **commands, char ***env);

typedef char	*(*t_function)(char *, t_list **, t_state_machine *);

typedef enum		e_cmd_code
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	ELSE
}					t_cmd_code;

typedef struct		s_all
{
	int				fd[3]; // a laisser ici
	t_cmd_code		command; // a retirer
	int				cmd_ret;
	uint8_t			pad[4];
	char			**env;
	t_list			*commands;
	char			**current; // a retirer
}					t_all;


typedef struct		s_exe
{
	t_cmd_code		cmd_code;
	uint8_t			pad[4];
	char			**args;
	char			***env;
}					t_exe;

typedef int			(*t_func)(t_exe *exe);

/*
**	**********************************
**	**			PROTOTYPES			**
**	**********************************
*/

/*
**	launch_command.c
*/
int					launch_command(t_exe *exe);

/*
**	all_init.c
*/
int					all_init(t_all *all);

/*
**	rec_gnl.c
*/
//int					rec_gnl(int fd, char **line);

/*
**	str_utils.c
*/
int					ft_strcmp(const char *s1, const char *s2);
int					ft_isset(char c, char *set);
char				*get_var_content(char **env, char *name);
ssize_t				get_var_pos(char **env, char *name);
void				ft_printarray_fd(char **array, int fd);
size_t				ft_arraylen(char **array);
void				free_str_array(char ***array);

/*
**	ft_split.c
**	char				**ft_split(char const *s, char set);
*/

/*
**	ft_exit.c
*/
int					ft_exit(int err_code, t_all *all);
int					err_bis(int err_code);

#endif

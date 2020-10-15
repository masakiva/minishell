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

# define METACHAR		"|;()<>"

# define BUILTINS		"echo/cd/pwd/export/unset/env/exit"

# define PROMPT			"$ "

/*
**	**********************************
**	**	CUSTOM TYPES AND STRUCTURES	**
**	**********************************
*/

enum		e_command
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	ELSE
};

//typedef struct		s_list
//{
//	char			*content;
//	struct s_list	*next;
//}					t_list;

typedef struct		s_all
{
	int				fd[3];
	enum e_command	command;
	char			**env;
	t_list			*stack;
	char			**current;
}					t_all;

typedef int			(*t_func)(t_all *all);

#include <stdio.h>
char			**split_command(char *line);

enum e_state
{
	LETTER,
	QUOTE,
	SPACE,
	//METACHAR,
	ERR,
	END,
	NB_STATES
};

# define METACHARS	"><|;"

# define BUF_SIZE	4096

typedef struct		s_state_machine
{
	enum e_state	state;
	uint8_t			pad[4];
	char			buf[BUF_SIZE];
	size_t			len;
	char			*cur_token;
}					t_state_machine;

void	print_token(void *token);
int		link_token(t_list **tokens, t_state_machine *machine);
int		reset_buf(t_state_machine *machine);
int		add_to_buf(char c, t_state_machine *machine);

# define FLAG_FILEIN	0b00000001 // <
# define FLAG_FILEOUT	0b00000010 // >
# define FLAG_APPEND	0b00000100 // >>
# define FLAG_PIPE		0b00001000 // |

//typedef struct		s_command
//{
//	t_list		*tokens; // liste de char *
//	//t_byte		flags;
//	//char		*file; // pour les flags > < ou >>
//}					t_command;

//typedef struct		s_all
//{
//	t_list		*commands; // separees par ;
//}					t_all;

typedef ssize_t		(*t_function)(char *, t_list **, t_state_machine *);

/*
**	**********************************
**	**			PROTOTYPES			**
**	**********************************
*/

/*
**	launch_command.c
*/
int					launch_command(t_all *all);

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
void				ft_printarray_fd(char **array, int fd);
size_t				ft_arraylen(char **array);
void				free_str_array(char ***array);

/*
**	ft_split.c
*/
char				**ft_split(char const *s, char *set);

/*
**	ft_exit.c
*/
int					ft_exit(int err_code, t_all *all);
int					err_bis(int err_code);

#endif

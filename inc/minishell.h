/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:29:47 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 13:17:12 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*
**	# define _XOPEN_SOURCE
*/

/*
**	**********************************
**	**			DEFINES				**
**	**********************************
*/
# define ISSPACE_3 "\f\n\r\t\v "

# define BUILTINS "echo/cd/pwd/export/unset/env/exit"

/*
**	**********************************
**	**	CUSTOM TYPES AND STRUCTURES	**
**	**********************************
*/
typedef enum		e_state
{
	PRM_INIT,
	GET_INPUT,
	LAUNCH,
	FT_EXIT
}					t_state;

typedef enum		e_error
{
	FALSE,
	ARG_ERR,
	MALLOC_ERR
}					t_error;

typedef enum		e_command
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	ELSE
}					t_command;

typedef struct		s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

typedef struct		s_param
{
	t_state		state;
	t_error		err_code;
	t_command	command;
	t_list		*env;
	char		**current;
}					t_param;

typedef int			(*t_func)(t_param *prm);

/*
**	**********************************
**	**			PROTOTYPES			**
**	**********************************
*/

/*
**	launch_command.c
*/
int					launch_command(t_param *prm);

/*
**	prm_init.c
*/
int					prm_init(t_param *prm);

/*
**	rec_gnl.c
*/
int					rec_gnl(int fd, char **line);

/*
**	str_utils.c
*/
void				ft_putchar(char c);
void				ft_putstr(const char *str);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_isset(char c, char *set);
char				*ft_strdup(const char *s1);

/*
**	lst_utils.c
*/
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **alst, t_list *new);
void				ft_lstadd_back(t_list **alst, t_list *new);
int					ft_lstsize(t_list *lst);
void				ft_lstprint(void *content);
void				ft_lstiter(t_list *lst, void (*f)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void*));
void				ft_lstdelone(t_list *lst, void (*del)(void *));

/*
**	ft_split.c
*/
char				**ft_split(char const *s, char *set);

/*
**	ft_exit.c
*/
int					ft_exit(t_param *prm);

#endif

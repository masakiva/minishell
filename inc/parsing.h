#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define BUF_SIZE		4096

enum e_state
{
	SPACE,
	LETTER,
//	QUOTE,
	BACKSLASH,
//	DOLLAR,
//	TILDE,
//	ANGLE_BRACKET,
	END,
	NB_STATES
};

enum e_parsing_error
{
	NO_ERROR,
	SQUOTE_MISSING,
	DQUOTE_MISSING,
	ESCAPE_NL,
	REDIR_PATH_INVALID,
	REDIR_PATH_MISSING,
	EMPTY_CMD,
	NB_PARSING_ERRORS
};

typedef struct		s_state_machine
{
	char			buf[BUF_SIZE];
	size_t			len;
	char			**args;
	char			*cur_arg;
	enum e_state	state;
	t_byte			pipe_flag;
	uint8_t			pad[3];
}					t_state_machine;

typedef char	*(*t_parse)(t_state_machine *, char *);

/*
**	main function
*/

char	**parse_one_command(char **line, t_byte *pipe_flag);

/*
**	utils
*/

int		new_command(t_list **commands);
int		add_variable(t_list **var_list, size_t start, size_t len, t_byte split_flag);
char	*parse_variable(char *line, t_state_machine *machine, t_byte split_flag);
int		add_arg(t_state_machine *machine);
int		reset_buf(t_state_machine *machine);
int		add_to_buf(t_state_machine *machine, char c);

#endif

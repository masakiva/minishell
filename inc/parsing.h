#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define BUF_SIZE		4096

enum e_state
{
	LETTER,
	QUOTE,
	BACKSLASH,
	DOLLAR,
	TILDE,
	SPACE,
	ANGLE_BRACKET,
	SEMICOLON,
	PIPE,
	//METACHAR, // a la place des n precedents
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
	enum e_state			state;
	enum e_parsing_error	error;
	char					buf[BUF_SIZE];
	size_t					len;
	t_token					*cur_token;
}							t_state_machine;

typedef char	*(*t_parse)(char *, t_list **, t_state_machine *);

/*
**	main function
*/

int		parse_input(char *line, t_list **commands);

/*
**	utils
*/

int		new_command(t_list **commands);
int		add_variable(t_list **var_list, size_t start, size_t len, t_byte split_flag);
char	*parse_variable(char *line, t_state_machine *machine, t_byte split_flag);
int		link_token(t_list **tokens, t_state_machine *machine);
int		reset_buf(t_state_machine *machine);
int		add_to_buf(char c, t_state_machine *machine);

#endif

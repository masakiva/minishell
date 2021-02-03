#ifndef ERROR_H
# define ERROR_H

#include <string.h>

#include "minishell.h"

enum		e_retcode
{
	_EXIT_CODE_ = 2,
	CLEAN_EXIT,
	ARG_ERR,
	FT_EXIT,
	PIPE_EXIT,
	CHILD_EXIT,
	CHILD_ERROR,
	_ERRNO_MSG_,
	MALLOC_ERR,
	WRITE_ERR,
	GNL_ERR,
	INVALID_CD_PATH,
	INVALID_CD_NEW_PATH,
	INVALID_PWD_PATH,
	INVALID_PATH_DIR,
	FD_ERROR,
	_EXEC_ERROR_,
	AMBIG_REDIR,
	HOME_NOT_SET,
	NO_SUCH_FILE,
	INVALID_EXP_ID,
	INVALID_UNS_ID,
	CD_ARG_ERR,
	EXIT_ARG_ERR,
	_PARSING_ERROR_,
	SQUOTE_MISSING,
	DQUOTE_MISSING,
	ESCAPE_NL,
	REDIR_PATH_INVALID,
	REDIR_PATH_MISSING,
	EMPTY_CMD
};

int			clean_and_exit(int ret, t_xe *xe);
int			ft_error(int ret, t_xe *xe);
int			error_and_exit(enum e_retcode ret, t_xe *xe);

#endif

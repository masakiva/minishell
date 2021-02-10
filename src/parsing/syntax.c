
#include "parsing.h"

int	check_backsl(unsigned int *flags)
{
	if (*flags & S_QUOTE)
		return (SUCCESS);
	if (!(*flags & S_BACKSL))
		*flags += S_BACKSL;
	return (SUCCESS);
}

int	check_quote(unsigned int *flags)
{
	if (*flags & S_DQUOTE)
		return (SUCCESS);
	else if (!(*flags & S_QUOTE))
		*flags += S_QUOTE;
	else
	{
		if (*flags & S_R_REDIR)
			*flags -= S_R_REDIR;
		else if (*flags & S_L_REDIR)
			*flags -= S_L_REDIR;
		else if (*flags & S_EMPTY)
			*flags -= S_EMPTY;
		*flags -= S_QUOTE;
	}
	return (SUCCESS);
}

int	check_dquote(unsigned int *flags)
{
	if (*flags & S_QUOTE)
		return (SUCCESS);
	else if (!(*flags & S_DQUOTE))
		*flags += S_DQUOTE;
	else
	{
		if (*flags & S_R_REDIR)
			*flags -= S_R_REDIR;
		else if (*flags & S_L_REDIR)
			*flags -= S_L_REDIR;
		else if (*flags & S_EMPTY)
			*flags -= S_EMPTY;
		*flags -= S_DQUOTE;
	}
	return (SUCCESS);
}

int	check_r_redir(unsigned int *flags)
{
	if ((*flags & S_R_REDIR))
	{
		*flags += S_APPEND;
		*flags -= S_R_REDIR;
	}
	else if (!(*flags & S_R_REDIR) && !(*flags & S_L_REDIR))
		*flags += S_R_REDIR;
	else
		return (REDIR_PATH_MISSING);
	return (SUCCESS);
}

int	check_l_redir(unsigned int *flags)
{
	if (!(*flags & S_L_REDIR) && !(*flags & S_R_REDIR))
		*flags += S_L_REDIR;
	else
		return (REDIR_PATH_MISSING);
	return (SUCCESS);
}

int	check_semicol(unsigned int *flags)
{
	if (!(*flags & S_SEMICOL) && !(*flags & S_PIPE))
		*flags += S_SEMICOL;
	else
		return (EMPTY_CMD);
	if (*flags & S_R_REDIR || *flags & S_L_REDIR)
		return (REDIR_PATH_MISSING);
	if (*flags & S_EMPTY)
		return (EMPTY_CMD);
	else
		*flags += S_EMPTY;
	return (SUCCESS);
}

int	check_pipe(unsigned int *flags)
{
	if (!(*flags & S_SEMICOL) && !(*flags & S_PIPE))
		*flags += S_PIPE;
	else
		return (EMPTY_CMD);
	if (*flags & S_R_REDIR || *flags & S_L_REDIR)
		return (REDIR_PATH_MISSING);
	if (*flags & S_EMPTY)
		return (EMPTY_CMD);
	else
		*flags += S_EMPTY;
	return (SUCCESS);
}

int	check_space(unsigned int *flags)
{
	(void)*flags;
	return (SUCCESS);
}

int	check_others(unsigned int *flags)
{
	if (*flags & S_R_REDIR)
		*flags -= S_R_REDIR;
	if (*flags & S_L_REDIR)
		*flags -= S_L_REDIR;
	if (*flags & S_APPEND)
		*flags -= S_APPEND;
	if (*flags & S_EMPTY)
		*flags -= S_EMPTY;
	if (*flags & S_PIPE)
		*flags -= S_PIPE;
	if (*flags & S_SEMICOL)
		*flags -= S_SEMICOL;
	return (SUCCESS);
}

int		final_checks(int flags)
{
	if (flags & S_BACKSL)
		return (ESCAPE_NL);
	if (flags & S_APPEND)
		return (REDIR_PATH_MISSING);
	if (flags & S_R_REDIR)
		return (REDIR_PATH_MISSING);
	if (flags & S_L_REDIR)
		return (REDIR_PATH_MISSING);
	if (flags & S_QUOTE)
		return (SQUOTE_MISSING);
	if (flags & S_DQUOTE)
		return (DQUOTE_MISSING);
	if (flags & S_PIPE)
		return (ESCAPE_NL);
	return (SUCCESS);
}

int		check_syntax(char *line)
{
	int				i;
	int				ret;
	unsigned int	flags;
	const t_syntax	syn_checks[] = {check_backsl, check_quote, check_dquote,
						check_r_redir, check_l_redir, check_semicol,
						check_pipe, check_space};

	i = 0;
	flags = S_EMPTY;
	while (line[i])
	{
		if (flags & S_BACKSL)
		{
			flags -= S_BACKSL;
			if (flags & S_EMPTY)
				flags -= S_EMPTY;
			if (flags & S_APPEND)
				flags -= S_APPEND;
			if (flags & S_R_REDIR)
				flags -= S_R_REDIR;
			if (flags & S_L_REDIR)
				flags -= S_L_REDIR;
			i++;
		}
		else if (flags & S_QUOTE && line[i] == '\'')
		{
			flags -= S_QUOTE;
			i++;
		}
		else if (flags & S_DQUOTE && (line[i] == '\'' || line[i] == '"'))
		{
			if (line[i] == '"')
				flags -= S_DQUOTE;
			if (line[i] == '\\')
				flags += S_BACKSL;
			i++;
		}
		else
		{
	//		ret = check_bsl_and_quotes(&flags, line[i]);
			ret = ft_set_index(line[i], METACHARS);
			if (ret >= 0)
				ret = syn_checks[ret](&flags);
			else
				ret = check_others(&flags);
			if (ret != SUCCESS)
				return (ret);
			i++;
		}
	}
	return (final_checks(flags));
}

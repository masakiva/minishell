/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:31:21 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:35:05 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int			final_checks(int flags)
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

static void	state_modifiers(unsigned int *flags, char c)
{
	if (*flags & S_BACKSL)
	{
		*flags -= S_BACKSL;
		if (*flags & S_EMPTY)
			*flags -= S_EMPTY;
		if (*flags & S_APPEND)
			*flags -= S_APPEND;
		if (*flags & S_R_REDIR)
			*flags -= S_R_REDIR;
		if (*flags & S_L_REDIR)
			*flags -= S_L_REDIR;
	}
	else if (*flags & S_QUOTE)
	{
		if (c == '\'')
			*flags -= S_QUOTE;
	}
	else if (*flags & S_DQUOTE)
	{
		if (c == '"')
			*flags -= S_DQUOTE;
		if (c == '\\')
			*flags += S_BACKSL;
	}
}

int			check_syntax(char *line)
{
	int				i;
	int				ret;
	unsigned int	flags;
	const t_syntax	syn_checks[] = {check_backsl, check_quote, check_dquote,
						check_r_redir, check_l_redir, check_semicol,
						check_pipe, check_space};

	i = 0;
	flags = S_EMPTY;
	while (line[i++])
	{
		if (flags & S_BACKSL || flags & S_QUOTE || flags & S_DQUOTE)
			state_modifiers(&flags, line[i]);
		else
		{
			ret = ft_set_index(line[i], METACHARS);
			if (ret >= 0)
				ret = syn_checks[ret](&flags);
			else
				ret = check_others(&flags);
			if (ret != SUCCESS)
				return (ret);
		}
	}
	return (final_checks(flags));
}

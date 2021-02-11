/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:31:14 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:35:22 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		*flags -= S_QUOTE;
	if (*flags & S_R_REDIR)
		*flags -= S_R_REDIR;
	else if (*flags & S_L_REDIR)
		*flags -= S_L_REDIR;
	else if (*flags & S_EMPTY)
		*flags -= S_EMPTY;
	return (SUCCESS);
}

int	check_dquote(unsigned int *flags)
{
	if (*flags & S_QUOTE)
		return (SUCCESS);
	else if (!(*flags & S_DQUOTE))
		*flags += S_DQUOTE;
	else
		*flags -= S_DQUOTE;
	if (*flags & S_R_REDIR)
		*flags -= S_R_REDIR;
	else if (*flags & S_L_REDIR)
		*flags -= S_L_REDIR;
	else if (*flags & S_EMPTY)
		*flags -= S_EMPTY;
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

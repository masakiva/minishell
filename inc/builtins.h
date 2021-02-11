/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:37:02 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:37:27 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "execution.h"
# include "error.h"

# define ECHO_OPTIONS	"n"
# define ECHO_N_OPTION	0x01

# define ENTIRE_STR	-1

/*
**	builtins
*/

int			ft_echo(char **args, t_xe *xe);
int			ft_pwd(char **args, t_xe *xe);
int			ft_cd(char **args, t_xe *xe);
int			ft_export(char **args, t_xe *xe);
int			ft_unset(char **args, t_xe *xe);
int			ft_env(char **args, t_xe *xe);
int			ft_exit(char **args, t_xe *xe);

/*
**	utils
*/

t_byte		shift_options(char ***args, char *op_chars);
int			print_export(char **env, char **exported);
int			ft_env_export(char *var, t_xe *xe, ssize_t equalsign_pos);
int			check_var_name(char *var, ssize_t name_len);

#endif

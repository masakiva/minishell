NAME		= minishell

SRC_DIR		+= src/
SRC_DIR		+= src/parsing/
SRC_DIR		+= src/execution/
SRC_DIR		+= src/builtins/

SRC	+= main.c
SRC	+= signal_handling.c

SRC += check_syntax.c
SRC	+= parsing_handler.c
SRC += parsing_states1.c
SRC += parsing_states2.c
SRC += parsing_vars_redirs.c
SRC	+= parsing_utils.c

SRC	+= exec_handler.c
SRC	+= launch_command.c

SRC	+= ft_echo.c
SRC	+= options.c
SRC	+= ft_pwd.c
SRC	+= ft_cd.c
SRC	+= ft_export.c
SRC	+= export_print.c
SRC	+= export_in_env.c
SRC	+= ft_unset.c
SRC	+= var_utils.c
SRC	+= ft_env.c
SRC	+= ft_exit.c

SRC	+= ft_error.c

vpath %.c $(SRC_DIR)

INC_DIR		= inc/

HDR	+= minishell.h
HDR	+= parsing.h
HDR	+= execution.h
HDR	+= builtins.h
HDR	+= error.h

vpath %.h $(INC_DIR)

OBJ_DIR		= obj/

OBJ			= $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

LIBFT_DIR	= libft/

CC			= clang

CFLAGS		+= -Wall
CFLAGS		+= -Werror
CFLAGS		+= -Wextra

CPPFLAGS	+= -I $(INC_DIR)
CPPFLAGS	+= -I $(LIBFT_DIR)

LDFLAGS		+= -L $(LIBFT_DIR)

LDLIBS		+= -lft

ifeq ($(d), 0)
	CFLAGS	+= -Wpadded
	CFLAGS	+= -g3
	CFLAGS	+= -fsanitize=address,undefined
	LDFLAGS	+= -fsanitize=address,undefined
endif

all:			$(NAME)

$(NAME):		$(LIBFT_DIR)libft.a $(OBJ)
				$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

$(OBJ_DIR)%.o:	%.c
				$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ):			$(HDR) $(LIBFT_DIR)libft.a | $(OBJ_DIR)

$(OBJ_DIR):
				mkdir $@

$(LIBFT_DIR)libft.a:
				$(MAKE) -C $(LIBFT_DIR) bonus custom

lib:
				$(MAKE) -C $(LIBFT_DIR) bonus custom
				$(MAKE) $(NAME)

clean:
				$(MAKE) -C $(LIBFT_DIR) fclean
				$(RM) -r $(OBJ_DIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re libft

NAME		= minishell

SRC_DIR		+= src/
SRC_DIR		+= src/parsing/
SRC_DIR		+= src/execution/

SRC	+= main.c
SRC	+= signal_handling.c
SRC	+= parse_input.c
SRC	+= parse_utils.c
SRC	+= exec_handler.c
#SRC	+= prepare_args.c
#SRC	+= test.c #alternative to prepare_args.c

SRC	+= launch_command.c
SRC	+= options.c
SRC	+= ft_echo.c
SRC	+= ft_pwd.c
SRC	+= ft_cd.c
SRC	+= var_utils.c
SRC	+= ft_export.c
SRC	+= ft_unset.c
SRC	+= ft_env.c

SRC	+= ft_exit.c

vpath %.c $(SRC_DIR)

INC_DIR		= inc/

HDR	+= minishell.h
HDR	+= signals.h
HDR	+= parsing.h
HDR	+= pre_execution.h
HDR	+= execution.h

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

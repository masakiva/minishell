NAME		= minishell

SRC_DIR		= src/

SRC_NAME	+= main.c
SRC_NAME	+= launch_command.c
SRC_NAME	+= ft_exit.c
SRC_NAME	+= str_utils.c
SRC_NAME	+= ft_split.c
SRC_NAME	+= rec_gnl.c
SRC_NAME	+= prm_init.c
SRC_NAME	+= parse_command.c
SRC_NAME	+= parse_utils.c

SRC			= $(addprefix $(SRC_DIR), $(SRC_NAME))

HDR_NAME	= minishell.h

INC_DIR		= inc/

HDR			= $(addprefix $(INC_DIR), $(HDR_NAME))

OBJ_DIR		= obj/

OBJ_NAME	= $(SRC_NAME:.c=.o)

OBJ			= $(addprefix $(OBJ_DIR), $(OBJ_NAME))

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

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
				$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ):			$(HDR) | $(OBJ_DIR)

$(OBJ_DIR):
				mkdir $@

$(LIBFT_DIR)libft.a:
				$(MAKE) -C $(LIBFT_DIR) bonus custom

clean:
				$(MAKE) -C $(LIBFT_DIR) fclean
				$(RM) -r $(OBJ_DIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re

CD_SRC = cd.c

CD_OBJ = $(CD_SRC:.c=.o)

INC_DIR = ../inc

CD = cd

CC = clang

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wextra

CPPFLAGS	+= -I $(INC_DIR)
CPPFLAGS	+= -I $(LIBFT_DIR)

LIBFT_DIR	= ../libft/

LDFLAGS		+= -L $(LIBFT_DIR)

LDLIBS		+= -lft

$(CD):	$(CD_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(CD_OBJ) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

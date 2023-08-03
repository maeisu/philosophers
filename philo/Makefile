NAME = philo

SRCS = main.c

OBJ_DIR = ./objs
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread -g
# CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I ./

RM = rm -rf

vpath %.c srcs/

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
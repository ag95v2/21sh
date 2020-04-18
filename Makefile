# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsorion <tsorion@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/12 15:20:15 by mphobos           #+#    #+#              #
#    Updated: 2020/04/18 12:48:13 by tsorion          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh
C = main.c signal.c input_mode.c
CREADLINE = readline.c add_or_delete_symbol.c expansion.c \
			termcap_api.c history_list.c reset_readline_position.c \
			history_file.c free_readline.c clear_all_line.c \
			other.c check_quoting.c search_cur_pos.c
CCURSOR_MANIPULATION = alt_left_right.c move_cursor.c wordmove_cursor.c
CTOKENIZING = get_next_token.c
SRC = $(patsubst %,src/%,$(C))
READLINE = $(patsubst %,src/readline/%,$(CREADLINE))
CURSOR_MANIPULATION = $(patsubst %,src/readline/cursor_manipulation/%,$(CCURSOR_MANIPULATION))
TOKENIZING = $(patsubst %,src/tokenizing/%,$(CTOKENIZING))
CC = gcc
INCLUDE = includes 
LIB_INC = libft/includes
CFLAGS = -Wall -Wextra -L libft -lft -ltermcap -I $(INCLUDE) -I $(LIB_INC) -o $(NAME)
DFLAGS = $(CFLAGS) -g

%.c:
	$(CC) $(SRC) $(READLINE) $(TOKENIZING) $(CURSOR_MANIPULATION) $(DFLAGS) -c $<

$(NAME): $(SRC) $(READLINE) $(TOKENIZING) $(CURSOR_MANIPULATION)
	make -C libft
	$(CC) $(SRC) $(READLINE) $(TOKENIZING) $(CURSOR_MANIPULATION) $(DFLAGS)

all: $(NAME)

clean:
	make -C libft clean

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

val:
	valgrind --leak-check=full ./21sh

.PHONY: all clean fclean re

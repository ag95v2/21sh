#include "21sh.h"

#ifdef DEBUG_INPUT_CHARS
#include <stdio.h>
#endif
static long readline_sup(void)
{
    long    c;

    c = 0;
    read(STDIN_FILENO, &c, sizeof(long));
	#ifdef DEBUG_INPUT_CHARS
	printf("%ld\n", c); // Compile with -DDEBUG_INPUT_CHARS
	fflush(stdout);		// To enable this part of code
	#endif
    if (c == LEFT_ARROW || c == RIGHT_ARROW)
        move_cursor(c);
    else if (c == UP_ARROW || c == DOWN_ARROW)
        up_down_arrow(c);
    else if (c == BACKSPACE)
        delete_symbol();
    else if (c == DEL)
        delete_symbol_forward();
    else if (c >= ' ' && c <= '~') // Probably, ft_isprint() here?
        add_symbol(c);
    else if (c == ALT_LEFT_ARROW || c == ALT_RIGHT_ARROW)
        alt_left_right(c);
    /*else if (c == HOME || c == END) // Необходимо реализовать
        home_end(c); // Проверить!*/
    else if (c == CTRL_LEFT || c == CTRL_RIGHT)
        wordmove_cursor(c);
	else if (c == CTRL_W)
		delete_last_word();
    return (c);
}

void	read_till_newline(int *user_in_len, \
		int tty)
{
	char	*nl;
    long    c;

	c = 0;
	if (tty)
	{
		while (c != '\n')
			c = readline_sup();
		*user_in_len = ft_strlen(rp()->user_in);
		rp()->user_in[*user_in_len] = c;
		rp()->user_in[*user_in_len + 1] = 0;
	}
	else
	{
		nl = 0;
		c = get_next_line(STDIN_FILENO, &nl);
		if (c == -1)
			exit(1);
		rp()->user_in[0] = 0;
		if (nl)
			ft_strncpy(rp()->user_in, nl, MAX_CMD_LENGTH - 2);
		rp()->user_in[MAX_CMD_LENGTH - 1] = 0;
		if ((*user_in_len = ft_strlen(nl)) > MAX_CMD_LENGTH - 1)
			exit(1);
		free(nl);
	}
}

/*
** Пероводит на новую строку и позволяет продолжить ввод
** для пользователя в случае, если цитирование не закрыто.
*/

static void readline_quoting(int tty)
{
    int     user_in_len;
    int     user_in_lines;

    rp()->cur_pos[0] = START_COL_POS;
    rp()->cur_pos[1] = START_ROW_POS;
	if (tty)
	{
        write(STDERR_FILENO, "\n", 1);
		write(STDERR_FILENO, "> ", 2);
	}
	read_till_newline(&user_in_len, tty);
	user_in_lines = str_n(rp()->user_in) - rp()->cur_pos[1];
	while (user_in_lines-- > 0)
		write(STDERR_FILENO, "\n", 1);
	check_flag(rp()->user_in, &(rp()->flag));
    if (rp()->flag != 0 || check_backslash(rp()->user_in, user_in_len - 1) == 0)
    {
        rp()->line_shift += user_in_len + 1;
        rp()->user_in += user_in_len + 1;
        readline_quoting(tty);
    }
}


/*
** Позволяет корректно работать со строкой ввода.
** Возвращает строку, введенную пользователем.
*/

char        *readline(int tty_input)
{
    char    *ret_user_in;
    int     user_in_len;
    int     user_in_lines;

	read_till_newline(&user_in_len, tty_input);
	user_in_lines = str_n(rp()->user_in) - rp()->cur_pos[1];
	while (user_in_lines-- > 0)
		write(STDERR_FILENO, "\n", 1);
	check_flag(rp()->user_in, &(rp()->flag));
    if (rp()->flag != 0 || check_backslash(rp()->user_in, user_in_len - 1) == 0)
    {
        rp()->line_shift += user_in_len + 1;
        rp()->user_in += user_in_len + 1;
        readline_quoting(tty_input);
    }
    rp()->user_in -= rp()->line_shift;
    rp()->user_in[ft_strlen(rp()->user_in) - 1] = 0;
    write(STDERR_FILENO, "\n", 1);
    if (!(ret_user_in = ft_strdup(rp()->user_in)))
        exit(1);
    return (ret_user_in);
}

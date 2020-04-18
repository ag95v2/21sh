#include "21sh.h"
#include <errno.h>

int			ft_putint(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}

void		test_tokenizing(char *user_in)
{
	TOKEN	token;
	char	token_type_name[13][BUFFSIZE] = {"NUMBER", "WORD",\
	"GREATER", "LESS", "GREATER_GREATER", "LESS_LESS",\
	"GREATER_AND", "LESS_AND", "AND_GREATER", "ASSIGNMENT_WORD",\
	"PIPE", "LINE_SEPARATOR", "END_LINE"};

	while (1)
	{
		token = get_next_token(user_in);
		if (token.token_type == WORD || token.token_type == NUMBER)
			printf("%s : %s\n", token_type_name[token.token_type], token.attribute);
		else
			printf("%s\n", token_type_name[token.token_type]);
		if (token.token_type == END_LINE)
			break ;
	}
}

void		init_terminal()
{
    char			*termtype;

	termtype = getenv("TERM");
	if (!termtype)
	{
		ft_putstr_fd("error\n", STDERR_FILENO); // Вывести соответствующее сообщение
		exit(1);
	}
	if (tgetent(NULL, termtype) != 1)
	{
		ft_putstr_fd("error\n", STDERR_FILENO); // Вывести соответствующее сообщение
		exit(1);
    }
    set_input_mode();
}

int			ret_winsize(int a)
{
	struct winsize	w;

	ioctl(STDERR_FILENO, TIOCGWINSZ, &w);
	if (!a)
		return (w.ws_col);
	return (w.ws_row);
}

t_rp		*readline_position(t_rp *change_rp)
{
	static t_rp	*rp;

	if (!change_rp)
		return (rp);
	else
		rp = change_rp;
	return (NULL);
}

t_rp		*init_rp(void)
{
	t_rp	*rp;

	if (!(rp = (t_rp*)malloc(sizeof(t_rp))))
		exit(1);
	if (!(rp->user_in = (char*)malloc(sizeof(char) * MAX_CMD_LENGTH)))
		exit(1);
	rp->cur_pos[0] = START_COL_POS;
	rp->cur_pos[1] = START_ROW_POS;
	rp->flag = 0;
	rp->line_shift = 0;
	rp->history = create_history("");
	rp->ws_col = ret_winsize(0);
	rp->ws_row = ret_winsize(1);
	return (rp);
}

void        start_program(char **env, int tty_input)
{
	char		*user_in;

	readline_position(init_rp());
	load_on_file_history(rp()->history);
	while (21)
	{
		reset_rp_to_start();
		if (tty_input)
			write(STDERR_FILENO, "$>", 2);
		user_in = readline(tty_input);
		add_to_start_history(rp()->history, user_in);
		if (!(ft_strcmp(user_in, "exit")))
			break ;
		free(user_in);
	}
	free(user_in);
	save_in_file_history(rp()->history);
	free_readline_position();
}

int         main(int ac, char **av, char **environ)
{
	int				tty_input;

    (void)ac;
    (void)av;
	tty_input = 0; /* Valgrind says it is uninitialized otherwise. Why? */
	if ((tty_input = isatty(STDIN_FILENO)))
		init_terminal();
	set_signal();
    start_program(environ, tty_input);
	if (tty_input)
		reset_input_mode();
    return (0);
}

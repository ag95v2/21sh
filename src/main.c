#include "21sh.h"
#include "static_env.h"

int			ft_putint(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}

void		set_input_mode()
{
	struct termios	new_settings;

	tcgetattr(STDIN_FILENO, &g_saved_attribute);
	new_settings = g_saved_attribute;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void	    reset_input_mode()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &g_saved_attribute);
}

void		init_terminal()
{
    char			*termtype;

	termtype = getenv("TERM");
	if (termtype == NULL || tgetent(NULL, termtype) != 1)
	{
		ft_putstr_fd("error\n", STDERR_FILENO);
		exit(1);
    }
    set_input_mode();
}

int			ret_winsize(int a)
{
	struct winsize	w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if (!a)
		return (w.ws_col);
	return (w.ws_row);
}

static void	processing_sigint(int signal_code)
{
	(void)signal_code;
	return ;
}

static void	processing_sigwinch(int signal_code)
{
	rp()->ws_col = ret_winsize(0);
	rp()->ws_row = ret_winsize(1);
}

/*
** Функция для обработки сигналов.
*/

void		set_signal(void)
{
	signal(SIGINT, processing_sigint);
	signal(SIGWINCH, processing_sigwinch);
}

void		back_to_start_history_rp(void)
{
	if (rp()->history)
		while (rp()->history->prev)
			rp()->history = rp()->history->prev;
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
		reset_exit(1);
	rp->len = 0;
	rp->max_len = MIN_CMD_LENGTH;
	rp->flag = 0;
	rp->line_shift = 0;
	rp->history = create_history("", 0);
	rp->ws_col = ret_winsize(0);
	rp->ws_row = ret_winsize(1);
	return (rp);
}

void		free_rp(void)
{
	free(rp()->user_in);
	free_history_list(rp()->history);
	free(rp());
}

void        start_program(int tty_input)
{
	t_deque		*command;

	(void)tty_input;
	command = NULL;
	readline_position(init_rp());
	load_on_file_history(rp()->history);
	while (1)
	{
		command = parser();
		exec_cmd(command);
	}
	save_in_file_history(rp()->history);
	free_rp();
}

void		reset_exit(int status)
{
	reset_input_mode();
	exit(status);
}

char		*set_programm_args(int c_ac, char **c_av)
{
	static int	ac;
	static char	**av;

	if (c_av)
	{
		ac = c_ac;
		av = c_av;
	}
	else if (c_ac >= ac || c_ac < 0)
		return (NULL);
	else
		return (av[c_ac]);
	return (NULL);
}

int         main(int ac, char **av, char **environ)
{
	int	tty_input;

	set_programm_args(ac, av);
	static_env_action(init, (void *)environ);
	if ((tty_input = isatty(STDIN_FILENO)))
		init_terminal();
	set_signal();
    start_program(tty_input);
	if (tty_input)
		reset_input_mode();
	static_env_action(del, NULL);
    return (0);
}

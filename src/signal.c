#include "21sh.h"

/*
** Функция для обработки сигналов.
*/

void		signal_processing(int signal_code)
{
	size_t	user_in_lines;

	if (signal_code == SIGINT)
	{
		write(STDERR_FILENO, "^C", 2);
		user_in_lines = str_n(rp()->user_in) + 2 - rp()->cur_pos[1];
		while (user_in_lines-- > 0)
			write(STDERR_FILENO, "\n", 1);
		rp()->user_in -= rp()->line_shift;
		write(STDERR_FILENO, "$>", 2);
		reset_rp_to_start();
	}
}

void		set_signal(void)
{
	signal(SIGINT, signal_processing);
}

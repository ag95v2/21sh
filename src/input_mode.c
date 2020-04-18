#include "21sh.h"

void		set_input_mode(void)
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

void	    reset_input_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &g_saved_attribute);
}

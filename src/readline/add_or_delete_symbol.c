#include "21sh.h"

/*
** Добавляет символ и печатает новую строку.
*/

void        add_symbol(char c)
{
    char    *str;
    int     tmp[2];
    int     i;

    i = search_index();
    str = ft_strdup(rp()->user_in + i);
    ft_strcpy(rp()->user_in + i + 1, str);
    free(str);
    rp()->user_in[i] = c;
    tmp[0] = rp()->cur_pos[0];
    tmp[1] = rp()->cur_pos[1];
    clear_all_line();
    rp()->cur_pos[0] = tmp[0] + 1;
    rp()->cur_pos[1] = tmp[1];
    ft_putstr_fd(rp()->user_in, STDERR_FILENO);
    if (rp()->cur_pos[0] == rp()->ws_col)
    {
        rp()->cur_pos[0] = 1;
        rp()->cur_pos[1]++;
        //write(STDERR_FILENO, "\n", 1);
        //printf("rp()->cur_pos[0] = %d, rp()->cur_pos[1] = %d\n", rp()->cur_pos[0], rp()->cur_pos[1]);
    }
    cur_pos_after_putstr(tmp);
    ret_cur_to_original_pos(tmp);
}

void		delete_symbol_forward(void)
{
    int tmp;

    tmp = rp()->cur_pos[0];
    move_cursor(RIGHT_ARROW);
    if (tmp != rp()->cur_pos[0])
	    delete_symbol();
}

int        delete_symbol_sup(int i)
{
    if (rp()->user_in[i] == '\n')
    {
        tc_cursor_up();
        if (rp()->cur_pos[1] == 1)
        {
            tc_cursor_right();
            tc_cursor_right();
            rp()->cur_pos[0] = 3;
        }
        return (search_last_cur_pos_in_line() + 2);
    }
    return (rp()->cur_pos[0]);
}

/*
** Удаляет символ и печатает новую строку.
*/

void        delete_symbol(void)
{
    int     len;
    int     i;
    int     tmp[2];

    if ((i = search_index() - 1) < 0)
        return ;
    tmp[0] = delete_symbol_sup(i);
    tmp[1] = rp()->cur_pos[1];
    clear_all_line();
    len = ft_strlen(rp()->user_in);
    while (i < len - 1)
    {
        rp()->user_in[i] = rp()->user_in[i + 1];
        i++;
    }
    rp()->user_in[i] = 0;
    rp()->cur_pos[0] = tmp[0] - 1;
    rp()->cur_pos[1] = tmp[1];
    ft_putstr(rp()->user_in);
    cur_pos_after_putstr(tmp);
    ret_cur_to_original_pos(tmp);
}

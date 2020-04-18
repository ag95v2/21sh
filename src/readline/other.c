#include "21sh.h"

int         search_index(void)
{
    int     n;
    int     i;

    n = 1;
    i = 0;
    if (rp()->cur_pos[1] == 1)
        return (rp()->cur_pos[0] - 3);
    while (n < rp()->cur_pos[1])
    {
        if (rp()->user_in[i] == '\n')
            n++;
        i++;
    }
    n = 1;
    while (n < rp()->cur_pos[0])
    {
        n++;
        i++;
    }
    return (i);
}

void        ret_cur_to_original_pos(int *prev_cur_pos)
{
    while (prev_cur_pos[0] > rp()->cur_pos[0])
    {
        tputs(tgetstr("le", NULL), STDERR_FILENO, ft_putint);
        prev_cur_pos[0]--;
    }
    while (prev_cur_pos[1] > rp()->cur_pos[1])
    {
        tputs(tgetstr("up", NULL), STDERR_FILENO, ft_putint);
        prev_cur_pos[1]--;
    }
    while (prev_cur_pos[1] < rp()->cur_pos[1])
    {
        write(STDERR_FILENO, "\n", 1);
        prev_cur_pos[1]++;
    }
    while (prev_cur_pos[0] < rp()->cur_pos[0])
    {
        tputs(tgetstr("nd", NULL), STDERR_FILENO, ft_putint);
        prev_cur_pos[0]++;
    }
}

void        delete_last_word(void)
{
    int i;

    while ((i = search_index() - 1) >= 0)
    {
        if (ft_isspace(rp()->user_in[i]))
		    delete_symbol();
        else
            break ;
    }
    while ((i = search_index() - 1) >= 0)
    {
        if (!ft_isspace(rp()->user_in[i]))
		    delete_symbol();
        else
            break ;
    }
}

/*
** This function probably should be added to libft
*/

int			ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\r' \
			|| c == '\t' || c == '\v')
		return (1);
	return (0);
}

//----------------------------------

int         str_n(char *user_in)
{
    int n;

    n = 0;
    while (*user_in != 0)
    {
        if (*user_in == '\n')
            n++;
        user_in++;
    }
    return (n);
}

void        up_down_arrow_sup(void)
{
    clear_all_line();
    ft_strcpy(rp()->user_in, rp()->history->str);
    ft_putstr(rp()->user_in);
    cur_pos_after_putstr(rp()->cur_pos);
}

void        up_down_arrow(long c)
{
    if (ft_strcmp(rp()->user_in, rp()->history->str) != 0)
    {
        free(rp()->history->str);
        rp()->history->str = ft_strdup(rp()->user_in);
    }
    if (c == UP_ARROW && rp()->history->next != NULL)
    {
        rp()->history = rp()->history->next;
        up_down_arrow_sup();
    }
    else if (c == DOWN_ARROW && rp()->history->prev != NULL)
    {
        rp()->history = rp()->history->prev;
        up_down_arrow_sup();
    }
}

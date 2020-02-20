#include "21sh.h"

/*
** Удаляет символ и печатает строку
*/

void        delete_symbol(char *user_in, int *cur_pos)
{
    int     len;
    int     i;

    if (*cur_pos == 3)
        return ;
    len = ft_strlen(user_in);
    i = 0;
    while (i < *cur_pos - 4)
        i++;
    while (i < len - 1)
    {
        user_in[i] = user_in[i + 1];
        i++;
    }
    user_in[i] = 0;
    clear_line(*cur_pos, 3);
    ft_putstr(user_in);
    while (len + 3 > *cur_pos)
    {
		tc_cursor_left(NULL);
        len--;
    }
    (*cur_pos)--;
}

void		delete_symbol_forward(char *user_in, int *cur_pos)
{
	if (!inside_boundaries(*cur_pos + 1, user_in))
		return ;
	tc_cursor_right(cur_pos);
	delete_symbol(user_in, cur_pos);
}

void        delete_last_word(char *user_in, int *cur_pos)
{
	while (ft_isspace(*input_under_cursor(*cur_pos, user_in)) &&\
			inside_boundaries(*cur_pos - 1, user_in))
		delete_symbol(user_in, cur_pos);
	while (!ft_isspace(*input_under_cursor(*cur_pos, user_in)) &&\
			inside_boundaries(*cur_pos - 1, user_in))
		delete_symbol(user_in, cur_pos);
}

/*
** Добавляет символ и печатает строку
*/

static void  insert_symbol_sup(char *user_in, int *cur_pos, char c, int a)
{
    int     len;
    char    *str;
    int     i;
    int     tmp[2];
    int     n;

    i = 0;
    n = 1;
    while (n != *(cur_pos + 1))
    {
        if (user_in[i] == '\n')
            n++;
        i++;
    }
    n = 1;
    while (n < *cur_pos - a)
    {
        n++;
        i++;
    }
    str = ft_strdup(user_in + i);
    ft_strcpy(user_in + i + 1, str);
    free(str);
    user_in[i] = c;
    tmp[1] = *cur_pos;
    tmp[2] = *(cur_pos + 1);
    clear_all_line(user_in, cur_pos);
    *(cur_pos) = tmp[1];
    *(cur_pos + 1) = tmp[2];
    ft_putstr(user_in);

    if (str_n(user_in) + 1 > *(cur_pos + 1))
    {
        i = str_n(user_in) + 1;
        while (i > *(cur_pos + 1))
        {
            tc_cursor_up(NULL);
            tc_cursor_up(NULL);
            write(STDOUT_FILENO, "\n", 1);
            i--;
        }
        i = 0;
        while (i < *(cur_pos))
        {
            tc_cursor_right(NULL);
            i++;
        }
    }
    else
    {
        i = ft_strlen(user_in);
        len = 0;
        while (user_in[i] != '\n')
        {
            i--;
            len++;
            if (i == 0)
                break ;
        }
        if (*(cur_pos + 1) != 1)
            len--;
        while (len + a > *cur_pos)
        {
		    tc_cursor_left(NULL);
            len--;
        }
    }
    (*cur_pos)++;
}

void        insert_symbol(char *user_in, int *cur_pos, char c)
{
    int n;
    int i;

    if (ft_strchr(user_in, '\n') == NULL)
        insert_symbol_sup(user_in, cur_pos, c, 2);
    else
    {
        i = 0;
        n = 1;
        while (n != *(cur_pos + 1))
        {
            if (user_in[i] == '\n')
                n++;
            i++;
        }
        if (n == 1)
        {
            insert_symbol_sup(user_in, cur_pos, c, 2);
            return ;
        }
        insert_symbol_sup(user_in, cur_pos, c, 0);
    }
}

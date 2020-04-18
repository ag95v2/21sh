#include "21sh.h"

/*
** Ищет последнюю позицию курсора по ширине для
** актуальной позиции курсора по высоте (rp()->cur_pos[1]). 
*/

int         search_last_cur_pos_in_line(void)
{
    int z;
    int p;
    int y;

    z = 1;
    p = 0;
    while (z != rp()->cur_pos[1])
    {
        if (rp()->user_in[p] == '\n')
            z++;
        p++;
    }
    y = 0;
    while (rp()->user_in[p] != '\n')
    {
        p++;
        y++;
    }
    if (rp()->cur_pos[1] == 1)
        return (y + 2);
    return (y);
}

/*
** Ищет и записывает в указатель cur_pos позицию
** курсора после вывода строки (rp()->user_in).
*/

void        cur_pos_after_putstr(int *cur_pos)
{
    int i;
    int n;

    cur_pos[1] = str_n(rp()->user_in) + 1;
    if (ft_strchr(rp()->user_in, '\n') == NULL)
        cur_pos[0] = ft_strlen(rp()->user_in) + 3;
    else
    {
        cur_pos[0] = 1;
        i = 0;
        n = cur_pos[1] - 1;
        while (n != 0)
        {
            if (rp()->user_in[i] == '\n')
                n--;
            i++;
        }
        while (rp()->user_in[i] != 0)
        {
            cur_pos[0]++;
            i++;
        }
    }
}

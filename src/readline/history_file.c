#include "21sh.h"

static int  open_21sh_history(int mode)
{
    int     fd;
    char    *home_dir;
    char    *file_path;

    if (!(home_dir = getenv("HOME")))
        return (-1);
    if (!(file_path = ft_strjoin(home_dir, "/.21sh_history")))
        return (-1);
    fd = open(file_path, mode, __S_IREAD | __S_IWRITE);
    return (fd);
}

void        load_on_file_history(t_history *history)
{
    int     fd;
    char    buf[1];
    char    str[BUFFSIZE];
    int     i;
    
    fd = open_21sh_history(O_RDONLY);
    if (fd == -1)
        return ;
    i = 0;
    while (read(fd, buf, 1) > 0)
    {
        if (buf[0] == '\n')
        {
            str[i] = 0;
            add_to_start_history(history, str);
            i = 0;
        }
        else
        {
            str[i] = buf[0];
            i++;
        }
    }
    close(fd);
}

static void save_in_file_history_sup(int fd, int n, char *history)
{
    int     i;
    char    buf[BUFFSIZE];

    i = 0;
    while (n != HISTSIZE)
    {
        if (history[i] == '\n')
            n--;
        i++;
    }
    ft_strcpy(buf, history + i);
    write(fd, buf, ft_strlen(buf));
    write(fd, "\n", 1);
}

void        save_in_file_history(t_history *history)
{
    int     fd;
    int     n;

    fd = open_21sh_history(O_WRONLY | O_TRUNC | O_CREAT);
    if (fd == -1)
        return ;
    while (history->prev)
        history = history->prev;
    n = 0;
    while (history->next && n < HISTSIZE)
    {
        history = history->next;
        n += str_n(history->str) + 1;
    }
    if (n > HISTSIZE)
    {
        save_in_file_history_sup(fd, n , history->str);
        if (history->prev)
            history = history->prev;
    }
    while (history->prev)
    {
        write(fd, history->str, ft_strlen(history->str));
        write(fd, "\n", 1);
        history = history->prev;
    }
    close(fd);
}
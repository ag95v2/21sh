#include "lex.h"

bool		is_letter(char c)
{
	bool	f;

	f = true;
	if (c == '>' || c == '<' || c == '|' ||\
	c == ' ' || c == '\t' || c == ';' ||\
	c == '\'' || c == 0 || c == '&' ||\
	c == '\"' || is_ws(c))
		f = false;
	return (f);
}

bool		is_digit(char c)
{
	bool	f;

	f = false;
	if (c >= '0' && c <= '9')
		f = true;
	return (f);
}

bool		is_ws(char c)
{
	bool	f;

	f = false;
	if (c == ' ' || c == '\t')
		f = true;
	return (f);
}

void		skip_ws(char *user_in, int *index)
{
	while (is_ws(user_in[*index]))
		(*index)++;
}

void		write_char_to_buf(char *user_in, int *index, t_str *attr)
{
	if (attr->max_len <= attr->len + 1)
		expand_str(attr);
	attr->buf[attr->index] = user_in[*index];
	(*index)++;
	attr->index++;
	attr->len++;
	attr->buf[attr->index] = '\0';
}

char		*create_attribute(t_str *attr)
{
	char	*attribute;

	attr->buf[attr->index] = 0;
	if (!(attribute = ft_strdup(attr->buf)))
		reset_exit(0);
	return (attribute);
}

/*
** returns a initialized copy of the next token
*/

static t_token	*copy_init_token(t_token t)
{
	t_token	*copy;

	copy = xmalloc(sizeof(t_token));
	*copy = t;
	if (copy->token_type != word &&\
			copy->token_type != ass_word &&\
			copy->token_type != number)
		copy->attribute = NULL;
	return (copy);
}

static void		call_lex_error(char flag)
{
	char	*prog_name;

	prog_name = get_prog_arg(0);
	if (!prog_name)
		reset_exit(1);
	ft_putstr_fd(prog_name, STDERR_FILENO);
	ft_putstr_fd(": unexpected EOF while looking for matching `", \
		STDERR_FILENO);
	write(STDERR_FILENO, &flag, 1);
	write(STDERR_FILENO, "'\n", 2);
	ft_putstr_fd(prog_name, STDERR_FILENO);
	ft_putstr_fd(": syntax error: unexpected end of file\n", STDERR_FILENO);
	reset_exit(1);
}

void			close_quote(char **user_in)
{
	char	*nuser_in;
	char	*tmp;
	char	*extra_line;
	char	flag;

	flag = 0;
	check_flag(*user_in, &flag);
	if (!flag)
		return ;
	while (flag)
	{
		if (isatty(STDIN_FILENO))
			extra_line = readline(get_prompt(PS2));
		else
		{
			extra_line = NULL;
			if (!(get_next_line(STDIN_FILENO, &extra_line)))
				call_lex_error(flag);
		}
		if (!(tmp = ft_strjoin(*user_in, "\n")))
			reset_exit(1);
		if (!(nuser_in = ft_strjoin(tmp, extra_line)))
			reset_exit(1);
		free(tmp);
		check_flag(extra_line, &flag);
		free(*user_in);
		free(extra_line);
		*user_in = nuser_in;
	}
}

t_token			ret_token_sup(char **user_in, int *index, t_str *attr)
{
	t_token	new_token;

	if ((*user_in)[*index] == '<')
		new_token = get_token_less(*user_in, index);
	else if ((*user_in)[*index] == '|')
		new_token = get_token_pipe(index);
	else if (is_letter((*user_in)[*index]))
		new_token = get_token_word(user_in, index, attr);
	else if ((*user_in)[*index] == ';')
		new_token = get_toket_line_separator(index);
	else if ((*user_in)[*index] == '\'')
	{
		close_quote(user_in);
		new_token = write_singe_quotes_to_buf(user_in, index, attr);
	}
	else if ((*user_in)[*index] == '&')
		new_token = get_token_and_greater(user_in, index, attr);
	else if ((*user_in)[*index] == '\"')
	{
		close_quote(user_in);
		new_token = write_double_quotes_to_buf(user_in, index, attr);
	}
	return (new_token);
}

t_token			*ret_token(char **user_in, int *need_new_line, int *index)
{
	static int	prev_token = -1;
	t_token		new_token;
	t_str		*attr;

	attr = init_str();
	skip_ws(*user_in, index);
	if (!(*user_in)[*index])
	{
		new_token = get_token_end_line(index);
		free(*user_in);
		*user_in = NULL;
		*need_new_line = 1;
	}
	else if (is_digit((*user_in)[*index]))
		new_token = get_token_number(user_in, index, attr, prev_token);
	else if ((*user_in)[*index] == '>')
		new_token = get_token_greater(*user_in, index);
	else
		new_token = ret_token_sup(user_in, index, attr);
	if (!(*need_new_line) && isatty(STDIN_FILENO))
		add_to_start_history(rp()->history, *user_in, ft_strlen(*user_in));
	prev_token = new_token.token_type;
	free_str(attr);
	return (copy_init_token(new_token));
}

t_token			*lex(void)
{
	static char	*user_in;
	static int	need_new_line;
	static int	index;

	if (syntax_error_state_action(SYNTAX_ERROR_STATE_GET, 0) == \
		SYNTAX_ERROR_STATE_NOT_OK)
	{
		free(user_in);
		user_in = 0;
		need_new_line = 1;
		index = 0;
		syntax_error_state_action(SYNTAX_ERROR_STATE_SET, \
			SYNTAX_ERROR_STATE_OK);
	}
	if (!user_in || need_new_line)
	{
		if (isatty(STDIN_FILENO))
			user_in = readline(DEFAULT_PROMPT);
		else if (!get_next_line(STDIN_FILENO, &user_in))
			reset_exit(0);
		need_new_line = 0;
	}
	return (ret_token(&user_in, &need_new_line, &index));
}

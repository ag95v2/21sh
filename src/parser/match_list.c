#include "parser.h"

int	match_list_dash(t_deque **command, t_deque **tokbuf_g)
{
	t_deque	*tokbuf_l;

	tokbuf_l = NULL;
	if (gett(tokbuf_g, &tokbuf_l)->token_type != sep)
	{
		ungett(tokbuf_g, &tokbuf_l);
		return (PARSER_SUCCESS);
	}
	if (match_pipeline(command, tokbuf_g) == PARSER_FAILURE)
	{
		ungett(tokbuf_g, &tokbuf_l);
		return (PARSER_SUCCESS);
	}
	match_list_dash(command, tokbuf_g);
	erase_tokbuf(&tokbuf_l);
	return (PARSER_SUCCESS);
}

int match_list(t_deque **command, t_deque **tokbuf_g)
{
	if (match_pipeline(command, tokbuf_g) == PARSER_FAILURE)
		return (PARSER_FAILURE);
	return (match_list_dash(command, tokbuf_g));
}
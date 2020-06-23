#include "parser.h"

/*
** Return type is void when we have EMPTY as one of the right parts of
** production
** Optimized tail recursion from 2 grammar rules.
** This is the only usage of nonterminal
*/

void	match_linebreak(t_deque **tokbubf_g, t_deque **tokbubf_l)
{
	while (1)
	{
		if (gett(tokbubf_g, tokbubf_l)->token_type == __newline)
			continue ;
		else
		{
			ungett(tokbubf_g, tokbubf_l);
			break ;
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anton <a@b>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 20:18:17 by anton             #+#    #+#             */
/*   Updated: 2020/06/27 01:43:25 by anton            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc21sh.h"

void		reset_exit(int status)
{
	if (isatty(0))
	{
		reset_input_mode();
		save_in_file_history(rp(NULL)->history);
	}
	exit(status);
}

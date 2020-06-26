/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anton <a@b>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 20:18:17 by anton             #+#    #+#             */
/*   Updated: 2020/06/24 21:18:24 by anton            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc21sh.h"

void		reset_exit(int status)
{
	if (isatty(0))
		reset_input_mode();
	back_to_start_history_rp();
	save_in_file_history(rp(NULL)->history);
	exit(status);
}

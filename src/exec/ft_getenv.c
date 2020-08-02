/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anton <a@b>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 01:29:13 by anton             #+#    #+#             */
/*   Updated: 2020/06/28 11:34:52 by anton            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

char			*ft_getenv(t_env env, char *name)
{
	t_ptree	*tree;

	tree = (t_ptree *)env;
	return (search_key(tree, name));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_mask.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 01:28:17 by mmonier           #+#    #+#             */
/*   Updated: 2019/06/16 18:46:47 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			msk(t_format *data, int mask)
{
	if (data->flag.opts & mask)
		return (1);
	return (0);
}

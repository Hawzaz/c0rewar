/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   way_to_corewar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 22:36:59 by mmonier           #+#    #+#             */
/*   Updated: 2019/09/21 03:14:22 by mmonier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cw_hexdump.h"

int		way_to_corewar(t_data *data)
{
	write_magic(data);
	write_header(data, NAME);
	write_header(data, COMMENT);
	write_opc(data, "ld");
	write_type(data);
	write_param(data, "42", 3);
	write_param(data, "1", 0);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmellon <bmellon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 23:42:45 by bmellon           #+#    #+#             */
/*   Updated: 2019/09/21 01:24:36 by bmellon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

// DIRECT_LOAD

void	op_ld(t_process *proc, unsigned char *arena, t_op *op)
{
	// load le 1er parametre dans le registre passé en 2nd parametre
	// si le 1st param = 0 le carry passe a 1
	t_param		params[3];

	get_params_len(&params, 2, op->types);
	get_param_data(&params, 2, (char *)proc->file, (REG_SIZE)proc->pc);
	proc->carry = params[1].data == 0 ? 1 : 0;
}

// DIRECT STORE

void	op_st(t_process *proc)
{
	// inverse de load charge le registre passe en 1st param dans le 2nd param
	// meme fonctionnement pour le carry
	int		secondparam;
	char	registre[REG_SIZE];

	get_reg_data(proc->file, 1);
	get_param(proc->file, 2);
	load_data((int)registre, secondparam);
	proc->carry = params[1].data == 0 ? 1 : 0;
}

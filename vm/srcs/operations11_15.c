/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations11_15.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 18:12:05 by bmellon           #+#    #+#             */
/*   Updated: 2019/10/07 21:27:16 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

extern struct s_op	op_tab[17];

/*
** STI 0x0B
** additione les deux derniers param et va changer la valeur a l'adresse de
** l'addition avec la valeur dans le registre passé en 1eme parametre
** si l'addition = 0 le carry passe a 1
*/

int		op_sti(t_param *params, t_process *proc, t_env *e)
{
	int		addr;

	if (params[1].type == REG_CODE)
		addr = *(REG_CAST *)proc->reg[params[1].value - 1];
	else if (params[1].type == DIR_CODE)
		addr = params[1].value;
	else
		addr = arena_get(e->arena, *(REG_CAST *)proc->pc + params[1].value,
			REG_SIZE);
	if (params[2].type == REG_CODE)
		addr += *(REG_CAST *)proc->reg[params[2].value - 1];
	else if (params[2].type == DIR_CODE)
		addr += params[2].value;
	addr = *(REG_CAST *)proc->pc + (addr % IDX_MOD);
	arena_copy(e->arena, addr, (REG_CAST *)proc->reg[params[0].value - 1],
		REG_SIZE);
	color_copy(e->colors, addr, proc->color[0], REG_SIZE);
	update_aff_arena(addr, REG_SIZE, *proc->color, e);
	return (addr);
}

/*
** FORK 0x0C
** Cree un nouveau processus
** fork un nouveau processus a l'adresse du premier parametre
** si l'adresse = 0 bah ca boucle du coup
*/

int		op_fork(t_param *params, t_process *proc, t_env *e)
{
	if (params[0].value != 0)
		proc->next = new_proc(proc, params[0].value, 0, e);
	return (proc->next != NULL);
}

/*
** LLD 0x0D
** direct load sans le %IDX_MOD
** si le 1st param = 0 le carry passe a 1
*/

int		op_lld(t_param *params, t_process *proc, t_env *e)
{
	if (params[0].type == DIR_CODE)
		*(REG_CAST *)proc->reg[params[1].value - 1] = params[0].value;
	else if (params[0].type == IND_CODE)
	{
		params[0].value = *(REG_CAST *)proc->pc + params[0].value;
		*(REG_CAST *)proc->reg[params[1].value - 1] =
			arena_get(e->arena, params[0].value, REG_SIZE);
	}
	return (*(REG_CAST *)proc->reg[params[1].value - 1]);
}

/*
** LLDI 0x0E
** ldi sans restriction d'adressage
** si l'addition = 0 le carry passe a 1
*/

int		op_lldi(t_param *params, t_process *proc, t_env *e)
{
	int			ret;
	int			addr;

	if (params[0].type == REG_CODE)
		ret = *(REG_CAST *)proc->reg[params[0].value - 1];
	else if (params[0].type == DIR_CODE)
		ret = params[0].value;
	else
		ret = arena_get(e->arena, *(REG_CAST *)proc->pc + params[0].value,
			REG_SIZE);
	if (params[1].type == REG_CODE)
		ret += *(REG_CAST *)proc->reg[params[1].value - 1];
	else
		ret += params[1].value;
	addr = *(REG_CAST *)proc->pc + ret;
	*(REG_CAST *)proc->reg[params[2].value - 1] =
		arena_get(e->arena, addr, REG_SIZE);
	return (*(REG_CAST *)proc->reg[params[2].value - 1]);
}

/*
** LFORK 0x0F
** fork un nouveau processus a l'adresse du premier parametre
** si l'adresse = 0 bah ca boucle aussi
*/

int		op_lfork(t_param *params, t_process *proc, t_env *e)
{
	if (params[0].value != 0)
		proc->next = new_proc(proc, params[0].value, 1, e);
	return (proc->next != NULL);
}
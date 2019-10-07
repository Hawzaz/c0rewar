/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations6_10.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 17:51:30 by bmellon           #+#    #+#             */
/*   Updated: 2019/10/07 19:58:27 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

extern struct s_op	op_tab[17];

/*
** AND 0x06
** AND avec le 1er param et le 2nd et stock le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

int		op_and(t_param *params, t_process *proc, t_env *e)
{
	int	and;

	(void)e;
	and = *(REG_CAST *)proc->reg[params[0].value - 1]
		& *(REG_CAST *)proc->reg[params[1].value - 1];
	*(REG_CAST *)proc->reg[params[2].value - 1] = and;
	return (and);
}

/*
** OR 0x07
** OR avec le 1er param et le 2nd et stock le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

int		op_or(t_param *params, t_process *proc, t_env *e)
{
	int	or;

	(void)e;
	or = *(REG_CAST *)proc->reg[params[0].value - 1]
		| *(REG_CAST *)proc->reg[params[1].value - 1];
	*(REG_CAST *)proc->reg[params[2].value - 1] = or;
	return (or);
}

/*
** XOR 0x08
** XOR avec le 1er param et le 2nd et stock le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

int		op_xor(t_param *params, t_process *proc, t_env *e)
{
	int	xor;

	(void)e;
	xor = *(REG_CAST *)proc->reg[params[0].value - 1]
		^ *(REG_CAST *)proc->reg[params[1].value - 1];
	*(REG_CAST *)proc->reg[params[2].value - 1] = xor;
	return (xor);
}

/*
** ZJMP 0x09
** jump a l'adresse donnee en parametre si le carry est a un, sinon fail
*/

int		op_zjmp(t_param *params, t_process *proc, t_env *e)
{
	(void)e;
	if (proc->carry == 1)
		params[0].size = params[0].value - 1;
	return (0);
}

/*
** LDI 0x0A
** additione les deux premiers param et va chercher a l'adresse de
** l'addition la valeur a ajouter dans le registre passé en 3eme parametre
** si l'addition = 0 le carry passe a 1
*/

int		op_ldi(t_param *params, t_process *proc, t_env *e)
{
	int			ret;
	int			addr;

	if (params[0].type == REG_CODE)
		ret = *(REG_CAST *)proc->reg[params[0].value - 1];
	else if (params[0].type == DIR_CODE)
		ret = params[0].value;
	else
		ret = arena_get(e->arena, *(REG_CAST *)proc->pc + params[0].value, REG_SIZE);
	if (params[1].type == REG_CODE)
		ret += *(REG_CAST *)proc->reg[params[1].value - 1];
	else
		ret += params[1].value;
	addr = ret % IDX_MOD;
	addr = calc_mod(*(REG_CAST *)proc->pc + addr, MEM_SIZE);
	*(REG_CAST *)proc->reg[params[2].value - 1] =
		arena_get(e->arena, addr, REG_SIZE);
	return (1);
}

/*
** Subject does not specify what we should return in op_ldi
**
** Zaz V.M. seems to always return 1
**
** return (*(REG_CAST *)proc->reg[params[2].value - 1]);
*/
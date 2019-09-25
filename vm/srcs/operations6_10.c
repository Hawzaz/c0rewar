/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations6_10.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmellon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 17:51:30 by bmellon           #+#    #+#             */
/*   Updated: 2019/09/23 03:20:20 by bmellon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

/*
** AND 0x06
** AND avec le 1er param et le 2nd et stock le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

void	op_and(t_process *proc, t_op *op, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 6);
	get_params_data(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc, *(REG_CAST *)proc->pc);
	*(REG_CAST *)proc->reg[params[2].value] = (params[0].value & params[1].value) % IDX_MOD;
	proc->carry = (params[0].value & params[1].value) == 0 ? 1 : 0;
	while (i < 3 && params[i].size != 0)
		len = params[i++].size;
	*((REG_CAST *)proc->pc) += len + 2;
}

/*
** OR 0x07
** OR avec le 1er param et le 2nd et stock le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

void	op_or(t_process *proc, t_op *op, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 7);
	get_params_data(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc, *(REG_CAST *)proc->pc);
	*(REG_CAST *)proc->reg[params[2].value] = (params[0].value | params[1].value) % IDX_MOD;
	proc->carry = (params[0].value | params[1].value) == 0 ? 1 : 0;
	while (i < 3 && params[i].size != 0)
		len = params[i++].size;
	*((REG_CAST *)proc->pc) += len + 2;
}

/*
** XOR 0x08
** XOR avec le 1er param et le 2nd et stock le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

void	op_xor(t_process *proc, t_op *op, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 8);
	get_params_data(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc, *(REG_CAST *)proc->pc);
	*(REG_CAST *)proc->reg[params[2].value] = (params[0].value ^ params[1].value) % IDX_MOD;
	proc->carry = (params[0].value ^ params[1].value) == 0 ? 1 : 0;
	while (i < 3 && params[i].size != 0)
		len = params[i++].size;
	*((REG_CAST *)proc->pc) += len + 2;
}

/*
** ZJMP 0x09
** jump a l'adresse donnee en parametre si le carry est a un, sinon fail
*/

void	op_zjmp(t_process *proc, t_op *op, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	if (proc->carry == 1)
	{
		get_params_len(params, 1, *((unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 9);
		get_params_data(params, 1, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc, *(REG_CAST *)proc->pc);
		*(REG_CAST *)proc->pc = params[0].value;
	}
	while (i < 3 && params[i].size != 0)
		len = params[i++].size;
	*((REG_CAST *)proc->pc) += len + 2;
}

/*
** LDI 0x0A
** additione les deux premiers param et va chercher a l'adresse de
** l'addition la valeur a ajouter dans le registre passé en 3eme parametre
** si l'addition = 0 le carry passe a 1
*/

void	op_ldi(t_process *proc, t_op *op, t_env *e)
{
	t_param		params[3];
	int			addr;
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 10);
	get_params_data(params, 3, (*(unsigned char *)e->arena + *(REG_CAST *)proc->pc, *(REG_CAST *)proc->pc);
	addr = (params[0].value + params[1].value) % IDX_MOD;
	*(REG_CAST *)proc->reg[params[2].value] = ((*(unsigned char *)e->arena + *(REG_CAST *)proc->pc)[addr];
	proc->carry = addr == 0 ? 1 : 0;
	while (i < 3 && params[i].size != 0)
		len = params[i++].size;
	*((REG_CAST *)proc->pc) += len + 2;
}
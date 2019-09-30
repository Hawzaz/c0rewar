/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmellon <bmellon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 23:42:45 by bmellon           #+#    #+#             */
/*   Updated: 2019/09/30 21:44:23 by bmellon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

extern t_op op_tab[17];

/*
** LIVE 0x01
** renvoie un live pour le processus qui l'appelle
*/

void	op_live(t_process *proc, t_env *e)
{
	t_param		params[3];
	t_process	*tail;

	get_params_len(params, 1,
		*((unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 1);
	get_params_data(params, 1,
		(unsigned char *)e->arena + *(REG_CAST *)proc->pc);
	tail = e->proc;
	while (tail)
	{
		if (params[0].value == tail->id)
		{
			tail->is_alive++;
			e->live.last_id = params[0].value;
			ft_strncpy(e->live.last_name, ((t_header *)tail->file)->prog_name,
							PROG_NAME_LENGTH + 1);
			break ;
		}
		tail = tail->next;
	}
	print_live(e, params, tail);
	proc->is_alive++;
	move_process_pc(proc, 6, e);
}

/*
** DIRECT_LOAD 0x02
** load le 1er parametre dans le registre passé en 2nd parametre
** si le 1st param = 0 le carry passe a 1
*/

void	op_ld(t_process *proc, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 2,
			*((unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 2);
	get_params_data(params, 2,
			(unsigned char *)e->arena + *(REG_CAST *)proc->pc);
	*(REG_CAST *)proc->reg[params[1].value - 1] = *(REG_CAST *)proc->pc +
		(params[0].value % IDX_MOD);
	proc->carry = (params[1].value == 0) ? 1 : 0;
	len = full_len_size(op_tab[1].reg_nb, params);
	move_process_pc(proc, len + 2, e);
}

/*
** DIRECT STORE 0x03
** inverse de load charge le registre passe en 1st param dans le 2nd param
** meme fonctionnement pour le carry
*/

void	op_st(t_process *proc, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 2,
			*((unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 3);
	get_params_data(params, 2,
			(unsigned char *)e->arena + *(REG_CAST *)proc->pc);
	if (params[1].size == 2)
		params[1].value = *(REG_CAST *)proc->pc +
			(*(REG_CAST *)proc->reg[params[0].value - 1] % IDX_MOD);
	else if (params[1].size == 1)
		params[1].value = *(REG_CAST *)proc->pc +
			(*(REG_CAST *)proc->reg[params[0].value - 1] % IDX_MOD);
	proc->carry = params[1].value == 0 ? 1 : 0;
	len = full_len_size(op_tab[2].reg_nb, params);
	move_process_pc(proc, len + 2, e);
}

/*
** ADD 0x04
** ajoute le 1er param et le 2nd et stocke le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

void	op_add(t_process *proc, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 3, *((unsigned char *)e->arena +
		*(REG_CAST *)proc->pc + 1), 4);
	get_params_data(params, 3, ((unsigned char *)e->arena) +
		*(REG_CAST *)proc->pc); 
	*(REG_CAST *)proc->reg[params[2].value] = params[0].value +
		params[1].value;
	proc->carry = params[0].value + params[1].value == 0 ? 1 : 0;
	len = full_len_size(op_tab[3].reg_nb, params);
	move_process_pc(proc, len + 2, e);
}

/*
** SUB 0x05
** soustraie le 1er param et le 2nd et stocke le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

void	op_sub(t_process *proc, t_env *e)
{
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	get_params_len(params, 3,
			*((unsigned char *)e->arena + *(REG_CAST *)proc->pc + 1), 5);
	get_params_data(params, 3,
			(unsigned char *)e->arena + *(REG_CAST *)proc->pc);
	*(REG_CAST *)proc->reg[params[2].value] = params[0].value -
		params[1].value;
	proc->carry = params[0].value - params[1].value == 0 ? 1 : 0;
	len = full_len_size(op_tab[4].reg_nb, params);
	move_process_pc(proc, len + 2, e);
}

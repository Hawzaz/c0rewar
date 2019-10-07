/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations1_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 23:42:45 by bmellon           #+#    #+#             */
/*   Updated: 2019/10/07 18:59:19 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

extern struct s_op	op_tab[17];

/*
** LIVE 0x01
** renvoie un live pour le processus qui l'appelle
*/

int		op_live(t_param *params, t_process *proc, t_env *e)
{
	t_process	*tail;

	tail = e->proc;
	while (tail)
	{
		if (params[0].value == tail->id)
		{
			tail->is_alive++;
			e->live.last_id = params[0].value;
			ft_strncpy(e->live.name, ((t_header *)tail->file)->prog_name,
							PROG_NAME_LENGTH + 1);
			break ;
		}
		tail = tail->next;
	}
	print_live(e, params, tail);
	proc->is_alive++;
	return (0);
}

/*
** DIRECT LOAD 0x02
** load le 1er parametre dans le registre passé en 2nd parametre
** si le 1st param = 0 le carry passe a 1
*/

int		op_ld(t_param *params, t_process *proc, t_env *e)
{
	int			len;

	if (params[0].type == REG_CODE)
	{
		len = calc_mod(*(REG_CAST *)proc->pc
			+ params[0].value % IDX_MOD, MEM_SIZE);
		*(REG_CAST *)proc->reg[params[1].value - 1] =
			arena_get(e->arena, len, 4);
	}
	else if (params[0].type == DIR_CODE)
		*(REG_CAST *)proc->reg[params[1].value - 1] = params[0].value;
	return (*(REG_CAST *)proc->reg[params[1].value - 1]);
}

/*
** DIRECT STORE 0x03
** inverse de load charge le registre passe en 1st param dans le 2nd param
** meme fonctionnement pour le carry
*/

int		op_st(t_param *params, t_process *proc, t_env *e)
{
	int		ret;

	if (params[1].type == REG_CODE)
	{
		*(REG_CAST *)proc->reg[params[1].value - 1] =
		*(REG_CAST *)proc->reg[params[0].value - 1];
	}
	else if (params[1].type == IND_CODE)
	{
		ret = *(REG_CAST *)proc->pc + (params[1].value % IDX_MOD);
		arena_copy(e->arena, ret, (REG_CAST *)proc->reg[params[0].value - 1],
			REG_SIZE);
		color_copy(e->colors, ret, proc->color[0], REG_SIZE);
		update_aff_arena(ret, REG_SIZE, *proc->color, e);
	}
	return (*(REG_CAST *)proc->reg[params[0].value - 1]);
}

/*
** ADD 0x04
** ajoute le 1er param et le 2nd et stocke le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

int		op_add(t_param *params, t_process *proc, t_env *e)
{
	int	add;

	(void)e;
	add = *(REG_CAST *)proc->reg[params[0].value - 1]
		+ *(REG_CAST *)proc->reg[params[1].value - 1];
	*(REG_CAST *)proc->reg[params[2].value - 1] = add;
	return (add);
}

/*
** SUB 0x05
** soustraie le 1er param et le 2nd et stocke le resultat dans le 3eme
** meme fonctionnement pour le carry
*/

int		op_sub(t_param *params, t_process *proc, t_env *e)
{
	int	sub;

	(void)e;
	sub = *(REG_CAST *)proc->reg[params[0].value - 1]
		- *(REG_CAST *)proc->reg[params[1].value - 1];
	*(REG_CAST *)proc->reg[params[2].value - 1] = sub;
	return (sub);
}

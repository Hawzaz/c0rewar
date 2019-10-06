/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_aff.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 19:23:33 by bmellon           #+#    #+#             */
/*   Updated: 2019/10/06 18:45:18 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

extern struct s_op	op_tab[17];

/*
** AFF 0x10
** Affiche le char qui est donne en 1er param si il est nul carry = 1
*/

int		op_aff(t_param *params, t_process *proc, t_env *e)
{
	if (e->ncu.info_win)
		wprintw(e->ncu.info_win, "corewar : [%s] : \"%c\"\n",
			((t_header *)proc->file)->prog_name, params[0].value);
	else
		ft_printf("corewar : [%s] : \"%c\"\n",
			((t_header *)proc->file)->prog_name, params[0].value);
	return (params[0].value);
}

void	print_live(t_env *e, t_param *params, t_process *tail)
{
	char		*proc;

	proc = "un processus dit que le joueur";
	if (e->ncu.info_win)
		wprintw(e->ncu.info_win, "%s: %s %d(%s) est en vie\n",
			e->progname, proc, params[0].value, (tail) ?
				((t_header *)tail->file)->prog_name : "?");
	else
		ft_printf("%s: %s %d(%s) est en vie\n",
			e->progname, proc, params[0].value, (tail) ?
				((t_header *)tail->file)->prog_name : "?");
}

int		handle_sti(t_param *params, t_process *proc, t_env *e)
{
	int		addr;

	addr = 0;
	if (params[1].type == REG_CODE
	&& params[1].value > 0 && params[1].value < REG_NUMBER)
		addr = *(REG_CAST *)proc->reg[params[1].value - 1];
	else if (params[1].type == IND_CODE)
		addr = arena_get(e->arena,
			calc_mod(*(REG_CAST *)proc->pc + 2 + params[1].value, MEM_SIZE),
			params[1].size);
	else if (params[1].type == DIR_CODE)
		addr += params[1].value;
	if (params[2].type == REG_CODE
	&& params[2].value > 0 && params[2].value < REG_NUMBER)
		addr += *(REG_CAST *)proc->reg[params[2].value - 1];
	else if (params[2].type == DIR_CODE)
		addr += params[2].value;
	addr %= IDX_MOD;
	arena_copy(e->arena, *(REG_CAST *)proc->pc + addr,
		(REG_CAST *)proc->reg[params[0].value - 1], REG_SIZE);
	color_copy(e->colors, *(REG_CAST *)proc->pc + addr,
		proc->color[0], REG_SIZE);
	update_aff_arena(*(REG_CAST *)proc->pc + addr, REG_SIZE, *proc->color, e);
	return (addr);
}

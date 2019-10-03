/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_aff.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 19:23:33 by bmellon           #+#    #+#             */
/*   Updated: 2019/10/03 22:31:07 by gbourgeo         ###   ########.fr       */
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

void	op_aff(t_process *proc, t_env *e)
{
	uint8_t		*arena;
	t_param		params[3];
	int			len;
	int			i;

	i = 0;
	arena = (uint8_t *)e->arena;
	get_params_len(params, 1,
		*(arena + (*(REG_CAST *)proc->pc + 1) % MEM_SIZE), 1);
	get_params_data(params, 1, arena, *(REG_CAST *)proc->pc);
	if (e->ncu.info_win)
		wprintw(e->ncu.info_win, "corewar : [%s] : \"%c\"\n",
			((t_header *)proc->file)->prog_name, params[0].value);
	else
		ft_printf("corewar : [%s] : \"%c\"\n",
			((t_header *)proc->file)->prog_name, params[0].value);
	proc->carry = params[0].value == 0 ? 1 : 0;
	len = full_len_size(op_tab[15].reg_nb, params);
	move_process_pc(proc, len + 2, e);
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

void	handle_st(t_param *params, t_process *proc, t_env *e)
{
	uint8_t		*arena;
	size_t		ret;

	arena = (uint8_t *)e->arena;
	if (params[1].size == 1)
	{
		if (params[1].value > 0 && params[1].value < REG_NUMBER)
			*(REG_CAST *)proc->reg[params[1].value - 1] =
			*(REG_CAST *)proc->reg[params[0].value - 1];
// wprintw(e->ncu.info_win, "storing r%d=%d into r%d(=%d)\n",
// params[0].value, *(REG_CAST *)proc->reg[params[0].value - 1], params[1].value, *(REG_CAST *)proc->reg[params[1].value - 1]);
	}
	else if (params[1].size == 2)
	{
		ret = *(REG_CAST *)proc->pc + params[1].value % IDX_MOD;
		ret = calc_mod(ret, MEM_SIZE);
// wprintw(e->ncu.info_win, "storing r%d=%d into pc=%d\n",
// params[0].value, *(REG_CAST *)proc->reg[params[0].value - 1], ret);
		arena_copy(e->arena, ret,
			(REG_CAST *)proc->reg[params[0].value - 1], REG_SIZE);
		color_copy(e->colors, ret, proc->color[0], REG_SIZE);
		update_aff_arena((char *)arena + ret, REG_SIZE, *proc->color, e);
	}
}

void	handle_sti(t_param *params, t_process *proc, t_env *e)
{
	uint8_t		*arena;
	size_t		ret;
	int			addr;

	arena = (uint8_t *)e->arena;
	if (params[1].size == 1)
		addr = ;
	else if (params[1].size == 2)
		addr = ;
	else
		addr = ;
	if (params[2].size == 2)
		addr += ;
	if (params[2].size == 4)
		addr += ;
	addr %= IDX_MOD;
	ret = *(REG_CAST *)proc->pc + (addr % IDX_MOD);
	ret = calc_mod(ret, MEM_SIZE);
	arena_copy(e->arena, ret,
	(REG_CAST *)proc->reg[params[0].value - 1], REG_SIZE);
	color_copy(e->colors, ret, proc->color[0], REG_SIZE);
	update_aff_arena((char *)arena + ret, REG_SIZE, *proc->color, e);
}

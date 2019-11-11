/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations16.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 19:23:33 by bmellon           #+#    #+#             */
/*   Updated: 2019/11/11 03:49:18 by gbourgeo         ###   ########.fr       */
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
	if (e->aff == TRUE)
	{
		if (e->ncu.info_win)
		{
			wprintw(e->ncu.info_win, "corewar : [%s] : \"%c\"\n",
				((t_header *)proc->file)->prog_name,
				*(REG_CAST *)proc->reg[params[0].value - 1]);
			wrefresh(e->ncu.info_win);
		}
		else
			ft_printf("corewar : [%s] : \"%c\"\n",
				((t_header *)proc->file)->prog_name, params[0].value);
	}
	return (params[0].value);
}

void	print_live(t_env *e, t_param *params, t_process *proc, t_process *tail)
{
	char		*str;

	str = "un processus dit que le joueur";
	if (e->ncu.info_win)
	{
		mvwprintw(e->ncu.champ_win, proc->pos_y + 2, 0, "Processes: %d, "
		"Lives: %d, Last Live: %d", *proc->free_file,
		e->live_live[proc->pos][0], e->live_live[proc->pos][1]);
		wrefresh(e->ncu.champ_win);
	}
	if (e->aff == TRUE)
	{
		if (e->ncu.info_win)
		{
			wprintw(e->ncu.info_win, "%s: %s %d(%s) est en vie\n",
				e->progname, str, params[0].value, (tail) ?
					((t_header *)tail->file)->prog_name : "?");
			wrefresh(e->ncu.info_win);
		}
		else
			ft_printf("%s: %s %d(%s) est en vie\n",
				e->progname, str, params[0].value, (tail) ?
					((t_header *)tail->file)->prog_name : "?");
	}
}

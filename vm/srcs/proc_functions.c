/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmellon <bmellon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 03:45:03 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/10/20 21:47:29 by bmellon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

static void		update_aff_processes(t_env *e)
{
	int			i;
	int			total;
	int			color;

	i = 0;
	total = 0;
	color = A_BOLD | COLOR_PAIR(COREWAR_CHAMPWIN_COLOR);
	if (e->ncu.champ_win)
	{
		while (i < MAX_PLAYERS)
			total += e->free_file[i++];
		i = e->ncu.winy - 1;
		wattron(e->ncu.champ_win, color);
		mvwprintw(e->ncu.champ_win, i, 0, "Processes: %d", total);
		wattroff(e->ncu.champ_win, color);
		wrefresh(e->ncu.champ_win);
	}
}

t_process		*new_proc(t_process *proc, int value, int flag, t_env *e)
{
	t_process	*new;

	new = ft_memalloc(sizeof(*new));
	ft_memcpy(new, proc, sizeof(*new));
	(*new->free_file)++;
	new->instruction_wait += 1;
	new->instruction = 0;
	if (!flag)
		move_process_pc(new, ((short)value % IDX_MOD), e);
	else
		move_process_pc(new, value, e);
	while (proc->prev && proc->prev->id == proc->id)
		proc = proc->prev;
	new->next = proc;
	if ((new->prev = proc->prev) != NULL)
		new->prev->next = new;
	else
		e->proc = new;
	proc->prev = new;
	update_aff_processes(e);
	return (new);
}

t_process		*remove_proc(t_process *proc, t_env *e)
{
	t_process	*next;

	next = proc->next;
	if (proc->prev)
		proc->prev->next = proc->next;
	else
		e->proc = proc->next;
	if (proc->next)
		proc->next->prev = proc->prev;
	if (proc->file && --(*proc->free_file) == 0)
		free(proc->file);
	ft_bzero(proc, sizeof(*proc));
	free(proc);
	update_aff_processes(e);
	return (next);
}

/*
** system("afplay ~/Desktop/roblox-death-sound-effect.mp3 2>&-");
*/

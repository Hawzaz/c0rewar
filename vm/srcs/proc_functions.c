/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 03:45:03 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/10/14 07:44:41 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_process		*new_proc(t_process *proc, int value, int flag, t_env *e)
{
	t_process *new;

	new = ft_memalloc(sizeof(*new));
	ft_memcpy(new, proc, sizeof(*new));
	(*new->free_file)++;
new->pos = *new->free_file; //
	new->instruction_wait += 1;
	new->instruction = 0;
	if (!flag)
		move_process_pc(new, ((short)value % IDX_MOD), e);
	else
		move_process_pc(new, value, e);
	new->prev = proc;
	if ((new->next = proc->next) != NULL)
		new->next->prev = new;
	if (e->ncu.champ_win)
	{
		wattron(e->ncu.champ_win, A_BOLD | COLOR_PAIR(COREWAR_CHAMPWIN_COLOR));
		mvwprintw(e->ncu.champ_win, proc->pos_y + 2, 0, "Processes: %d",
		*proc->free_file);
		wattroff(e->ncu.champ_win, A_BOLD | COLOR_PAIR(COREWAR_CHAMPWIN_COLOR));
		wrefresh(e->ncu.champ_win);
	}
	return (new);
}

t_process		*remove_proc(t_process *proc, t_process **head, t_env *e)
{
	t_process	*next;

	next = proc->next;
	if (proc->prev)
		proc->prev->next = proc->next;
	else
		*head = next;
	if (proc->next)
		proc->next->prev = proc->prev;
	if (proc->file && --(*proc->free_file) == 0)
		free(proc->file);
	ft_bzero(proc, sizeof(*proc));
	free(proc);
	if (e->ncu.champ_win)
	{
		wattron(e->ncu.vm_win, A_BOLD);
		wattron(e->ncu.champ_win, A_BOLD | COLOR_PAIR(COREWAR_CHAMPWIN_COLOR));
		mvwprintw(e->ncu.champ_win, proc->pos_y + 2, 0, "Processes: %d",
		*proc->free_file);
		wattroff(e->ncu.champ_win, A_BOLD | COLOR_PAIR(COREWAR_CHAMPWIN_COLOR));
		wrefresh(e->ncu.champ_win);
	}
	return (next);
}

/*
** system("afplay ~/Desktop/roblox-death-sound-effect.mp3 2>&-");
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_play.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 23:05:11 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/10/14 08:53:19 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static void		check_live_total(size_t nb_live, int *cycle_to_die)
{
	if (nb_live == NBR_LIVE)
		if ((*cycle_to_die = *cycle_to_die - CYCLE_DELTA) < 0)
			*cycle_to_die = 0;
}

static void		check_max_checks(int *checks, int *cycle_to_die)
{
	if (++(*checks) == MAX_CHECKS)
	{
		if ((*cycle_to_die = *cycle_to_die - CYCLE_DELTA) < 0)
			*cycle_to_die = 0;
		*checks = 0;
	}
}

static size_t	check_players_alive(t_env *e)
{
	t_process	*proc;
	size_t		alive;

	alive = 0;
	proc = e->proc;
	while (proc)
	{
		if (!proc->is_alive)
		{
			update_aff_champion_dead(e, proc);
			proc = remove_proc(proc, &e->proc, e);
			continue ;
		}
		alive += proc->is_alive;
		proc->is_alive = 0;
		proc = proc->next;
	}
	check_live_total(alive, &e->cycle_to_die);
	check_max_checks(&e->checks, &e->cycle_to_die);
	return (alive);
}

int				play_game(t_env *e)
{
	t_process	*proc;

	if (e->dump_cycle == e->nb_cycles)
	{
		return (-3);
	}
	if (e->nb_cycles && e->nb_cycles % e->cycle_to_die == 0)
		if (!check_players_alive(e))
			return (-2);
	proc = e->proc;
	while (proc)
	{
		proc->instruction_wait += player_instruction(proc, e);
		proc = proc->next;
	}
	return (0);
}

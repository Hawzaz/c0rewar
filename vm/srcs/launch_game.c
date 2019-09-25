/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_game.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 21:22:14 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/23 21:22:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

extern t_op op_tab[17];

static void			check_live_total(t_live *live, int *cycle_to_die)
{
	if (live->total == NBR_LIVE)
		if ((*cycle_to_die = *cycle_to_die - CYCLE_DELTA) < 0)
			*cycle_to_die = 0;
	live->total = 0;
}

static void			check_max_checks(int *checks, int *cycle_to_die)
{
	if (++(*checks) == MAX_CHECKS)
	{
		if ((*cycle_to_die = *cycle_to_die - CYCLE_DELTA) < 0)
			*cycle_to_die = 0;
		*checks = 0;
	}
}

static int			check_players_alive(t_process **head, t_env *e)
{
	t_process	*proc;
	int			alive;

	alive = 0;
	proc = *head;
	while (proc)
	{
		alive += proc->is_alive;
		if (!proc->is_alive)
			proc = remove_player(proc, head);
		else
			proc = proc->next;
	}
	check_live_total(&e->live, &e->cycle_to_die);
	check_max_checks(&e->checks, &e->cycle_to_die);
	return (alive);
}

static size_t	player_instruction(t_process *proc, t_env *e, size_t nb_cycles)
{
	static void	(*instruction_function[])(t_process *, t_op *, t_env *) = {
		// op_live, op_live, op_ld, op_st, op_add, op_sub, op_and, op_or, op_xor,
		// op_zjmp, op_ldi, op_sti, op_fork, op_lld, op_ldi, op_lfork, op_aff,
		NULL,
	};

	if (proc->instruction_wait > nb_cycles)
		return (1);
	if (proc->instruction_wait == nb_cycles)
	{
		if (proc->instruction == 0)
		{
			proc->instruction = *((unsigned char *)e->arena + *(REG_CAST *)proc->pc);
			if (proc->instruction > 0
			&& proc->instruction < (int)(sizeof(op_tab) / sizeof(op_tab[0])))
				ft_printf("instruction : %d IN RANGE\n", proc->instruction);// proc->instruction_wait += op_tab[proc->instruction - 1].cycle;
			else
			{
				proc->instruction = 0;
				if ((*(int *)proc->pc += 1) > MEM_SIZE)
					*(int *)proc->pc = 0;
			}
		}
		else
			(void)instruction_function;// instruction_function[proc->instruction](proc, op_tab, e);
	}
	return (1);
}

void			launch_game(t_env *e)
{
	t_process	*proc;
	size_t		nb_cycles;

	nb_cycles = 0;
	while (1)
	{
		proc = e->proc;
		if (e->dump_cycle > -1 && (size_t)e->dump_cycle == nb_cycles)
		{
			dump_map(e->arena, MEM_SIZE);
			return ;
		}
		if ((nb_cycles + 1) % e->cycle_to_die == 0)
			if (!check_players_alive(&e->proc, e))
				break ;
		while (proc)
		{
			proc->instruction_wait += player_instruction(proc, e, nb_cycles);
			proc = proc->next;
		}
		nb_cycles++;
	}
	if (e->live.last_id)
		ft_printf("le joueur %d(%s) a gagne\n", e->live.last_id, e->live.last_name);
	else
		ft_printf("Aucun champion n'a gagne... Vraiment !?\n");
}

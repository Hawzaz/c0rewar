/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_launch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 21:48:51 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/30 23:39:39 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

static void		print_win_str(t_ncurse *ncu, t_live *live)
{
	char		*str;
	char		*quit;
	int			len;
	int			x;

	str = (live->last_id) ? "Le joueur %d(%s) a gagné !" :
							"Aucun champion n'a gagné.";
	quit = "Press 'q' to quit...";
	len = ft_strlen(str);
	if (live->last_id)
		len += ft_strlen(live->name);
	x = (len > ncu->winx) ? 0 : ncu->winx / 2 - len / 2;
	wattron(ncu->info_win, COLOR_PAIR(COREWAR_WINNER_COLOR));
	mvwprintw(ncu->info_win, ncu->winy - 2, x, str, live->last_id, live->name);
	wattroff(ncu->info_win, COLOR_PAIR(COREWAR_WINNER_COLOR));
	mvwprintw(ncu->info_win, ncu->winy - 1, 0, quit);
}

/*
** MC HAMMER
*/

static void		and_the_winner_is(t_ncurse *ncu, t_live *live)
{
	if (ncu->info_win)
	{
		print_win_str(ncu, live);
		wrefresh(ncu->info_win);
		nodelay(ncu->info_win, FALSE);
		while (wgetch(ncu->info_win) != 'q')
			print_win_str(ncu, live);
		return ;
	}
	if (live->last_id)
		ft_printf("Le joueur %d(%s) a gagné\n", live->last_id, live->name);
	else
		ft_printf("Aucun champion n'a gagné.\n");
}

static void		set_game_parameters(t_env *e)
{
	e->pause = 1;
	e->speed = VM_SPEED_INIT;
	e->cycle_to_die = CYCLE_TO_DIE;
}

void			launch_game(t_env *e)
{
	size_t		nb_cycles;
	int			ch;

	nb_cycles = 0;
	ch = 0;
	set_game_parameters(e);
	ncurses_aff_all(e);
	while (1)
	{
		if (e->ncu.info_win)
		{
			update_aff_vminfo(e, nb_cycles);
			ch = ncurses_wgetch(&e->speed, &e->pause, e->ncu.info_win);
			if (ch == ERR)
				return ;
			else if (ch == 0)
				continue ;
		}
		if ((ch = play_game(nb_cycles, e)) != 0)
			break ;
		nb_cycles++;
	}
	if (ch == -2)
		and_the_winner_is(&e->ncu, &e->live);
}

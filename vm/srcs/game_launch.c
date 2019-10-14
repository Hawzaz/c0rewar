/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_launch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 21:48:51 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/10/14 08:44:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

static void		print_win_str(int ch, t_ncurse *ncu, t_live *live)
{
	char		*str;
	char		*quit;
	int			len;
	int			x;

	if (ch == -1)
		str = "ncurses: wgetch failed.";
	else if (ch == -2)
		str = (live->last_id) ? "Le joueur %d(%s) a gagné !" :
								"Aucun champion n'a gagné.";
	else
		str = "Dump !";
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

static void		and_the_winner_is(int ch, t_ncurse *ncu, t_live *live, t_env *e)
{
	if (ncu->info_win)
	{
		print_win_str(ch, ncu, live);
		if (ch == -3)
			return ;
		wrefresh(ncu->info_win);
		nodelay(ncu->info_win, FALSE);
		while (wgetch(ncu->info_win) != 'q')
			print_win_str(ch, ncu, live);
		return ;
	}
	if (ch == -1)
		corewar_errors(ERR_NCURSE_WGETCH, NULL, e);
	else if (ch == -2)
	{
		if (live->last_id)
			ft_printf("Le joueur %d(%s) a gagné\n", live->last_id, live->name);
		else
			ft_printf("Aucun champion n'a gagné.\n");
	}
}

static void		set_game_parameters(t_env *e)
{
	e->ch = 'p';
	e->pause = 1;
	e->speed = VM_SPEED_INIT;
	e->cycle_to_die = CYCLE_TO_DIE;
	ncurses_aff_all(e);
}

static t_bytes	*check_bytes(t_env *e, int cycle)
{
	t_bytes		*ret;
	t_bytes		*byte;
	t_bytes		*next;

	ret = e->bytes;
	byte = e->bytes;
	while (byte)
		if (byte->cycle_to_print <= cycle)
		{
			update_aff_arena(byte->offset, REG_SIZE,
			(short[2]){1, byte->color}, e);
			if (byte == ret)
				ret = byte->next;
			if (byte->prev)
				byte->prev->next = byte->next;
			if (byte->next)
				byte->next->prev = byte->prev;
			next = byte->next;
			free(byte);
			byte = next;
		}
		else
			byte = byte->next;
	return (ret);
}

void			launch_game(t_env *e)
{
	int			ret;

	ret = 0;
	set_game_parameters(e);
	while (ret == 0)
	{
		if (e->ncu.active == TRUE)
		{
			update_aff_vminfo(e);
			ret = ncurses_wgetch(e);
			if (ret == ERR)
				break ;
			else if (ret == 0)
				continue ;
		}
		if (!(ret = play_game(e)))
			e->nb_cycles++;
		e->bytes = check_bytes(e, e->nb_cycles);
	}
	and_the_winner_is(ret, &e->ncu, &e->live, e);
}

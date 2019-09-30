/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ncurses_init_end.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 23:28:36 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/30 23:55:08 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vm.h"
#include "libft.h"

/*
** Available colors:
** 	COLOR_BLACK		0
** 	COLOR_RED		1
** 	COLOR_GREEN		2
** 	COLOR_YELLOW	3
** 	COLOR_BLUE		4
** 	COLOR_MAGENTA	5
** 	COLOR_CYAN		6
** 	COLOR_WHITE		7
*/

static void		init_colors(t_env *e)
{
	static short	color_combo[] = {
		COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_RED,
	};
	short			i;
	int				color_nb;
	t_process		*proc;

	start_color();
	use_default_colors();
	init_pair(COREWAR_DFLT_COLOR, COLOR_BLACK, -1);
	init_pair(COREWAR_TITLE_COLOR, COLOR_YELLOW, -1);
	init_color(8, 100, 0, 200);
	init_pair(COREWAR_CHAMPWIN_COLOR, COLOR_WHITE, 8);
	init_color(9, 75, 0, 125);
	init_pair(COREWAR_INFOWIN_COLOR, COLOR_WHITE, 9);
	init_pair(COREWAR_TEXT_COLOR, COLOR_WHITE, -1);
	init_pair(COREWAR_WINNER_COLOR, COLOR_BLACK, COLOR_BLUE);
	i = COREWAR_COLOR_END;
	color_nb = 0;
	proc = e->proc;
	while (proc)
	{
		proc->color[0] = i;
		init_pair(i++, color_combo[color_nb], -1); // Player color
		proc->color[1] = i;
		init_pair(i++, COLOR_BLACK, color_combo[color_nb]); // Player color PC
		color_nb++;
		proc = proc->next;
	}
}

int				ncurses_init(t_env *e)
{
	int			err;

	err = IS_OK;
	if (e->ncu.active == FALSE)
		return (IS_OK);
	signal(SIGWINCH, ncurses_resizewindow);
	if ((e->ncu.main_win = initscr()) == NULL)
		return (ERR_NCURSE_MAINWIN);
	noecho();
	cbreak();
	keypad(e->ncu.main_win, TRUE);
	curs_set(0);
	init_colors(e);
	if (COLS < ARENA_LINE_LEN || LINES < ARENA_COL_LEN)
		e->term_too_small = ncurses_termtoosmall(e);
	else if ((err = create_arenabox(e)) == IS_OK)
		err = create_infobox(e);
	return (err);
}

/*
** int delwin(WINDOW *win);
**
** Calling delwin deletes the named window, freeing all memory associated with
** it (it does not actually erase the window's screen image).
** Subwindows must be deleted before the main window can be deleted.
*/

void			ncurses_end(t_env *e)
{
	if (e->ncu.arena_win)
		delwin(e->ncu.arena_win);
	if (e->ncu.arena_winbox)
		delwin(e->ncu.arena_winbox);
	if (e->ncu.champ_win)
		delwin(e->ncu.champ_win);
	if (e->ncu.vm_win)
		delwin(e->ncu.vm_win);
	if (e->ncu.info_win)
		delwin(e->ncu.info_win);
	if (e->ncu.info_winbox)
		delwin(e->ncu.info_winbox);
	endwin();
	ft_memset(&e->ncu, 0, sizeof(e->ncu));
}

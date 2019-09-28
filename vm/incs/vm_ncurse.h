/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_ncurse.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 23:40:54 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/29 00:13:21 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CW_NCURSE_H
# define CW_NCURSE_H

# include <ncurses.h>
# include "op.h"

# define VM_SPEED_INIT			20
# define VM_SPEED_RATIO			5
# define VM_SPEED_LIMIT			500

# define ARENA_VALUE_PER_LINE	64
# define ARENA_LINE_LEN			(ARENA_VALUE_PER_LINE * 3)
# define ARENA_COL_LEN			(MEM_SIZE / ARENA_VALUE_PER_LINE)

typedef struct			s_ncurse
{
	WINDOW				*mainWin;
	WINDOW				*arenaWinBox;
	WINDOW				*arenaWin;
	WINDOW				*infoWinBox;
	WINDOW				*champWin;
	WINDOW				*vmWin;
	WINDOW				*infoWin;
	int					winx;
	int					winy;
}						t_ncurse;

int						ncurses_wgetch(int *speed, int *pause, WINDOW *infoWin);

#endif

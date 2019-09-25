/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmellon <bmellon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 18:05:10 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/23 02:11:01 by bmellon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"
#include "ft_printf.h"

static int		usage(char *progname)
{
	ft_dprintf(2, "Usage : %s ", progname);
	ft_dprintf(2, "[-dump nbr_cycles] [[-n number] champion1.cor] ...\n\n");
	ft_dprintf(2, "\t-dump nbr_cycles\t: ");
	ft_dprintf(2, "Dumps memory after 'nbr_cycles' cycles then exits.\n");
	ft_dprintf(2, "\t-n number\t\t: ");
	ft_dprintf(2, "Fix a number to player. If missing, the player will ");
	ft_dprintf(2, "have the first available number.\n");
	return (1);
}

static void		introduce_champ(t_process *proc, t_env *e)
{
	t_header	*play;

	if (!e->pid)
		ft_printf("Introducing contestants...\n");
	else
	{
		wclear(e->ncu.infoLine);
		wrefresh(e->ncu.infoLine);
	}
	while (proc->next)
		proc = proc->next;
	while (proc)
	{
		play = (t_header *)proc->file;
		ft_printf("* Player %d, weighting %d bytes, \"%s\" (\"%s\") !\n",
		proc->id, proc->data_size, play->prog_name, play->comment);
		proc = proc->prev;
	}
}

int				main(int ac, char **av)
{
	t_env	*e;

	e = &g_env;
	ft_memset(e, 0, sizeof(*e));
	e->progname = ft_strrchr(av[0], '/');
	e->progname = (e->progname) ? e->progname + 1 : av[0];
	e->dump_cycle = -1;
	if (ac < 2 || get_args(av, e) || get_arena(e))
		return (usage(av[0]));
	introduce_champ(e->proc, e);
	signal(SIGINT, corewar_end);
	launch_game(e);
	if (e->pid)
		waitpid(e->pid, NULL, 0);
	free_env(e);
}

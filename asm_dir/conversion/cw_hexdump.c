/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cw_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 18:03:52 by mmonier           #+#    #+#             */
/*   Updated: 2019/09/28 01:23:01 by mmonier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cw_hexdump.h"

static void		print_head(t_data *data)
{
	char		c;
	int			pause;

	pause = 0;
	while (1)
	{
		if (pause == 0)
		{
			print_file_name(data);
			print_dot(data);
			print_compilation(data);
			pause = 1;
		}
		if ((c = wgetch(data->main_win)) == '\n')
			break ;
	}
}

static int				cw_hexdump(t_data *data, char *file)
{
	if ((data->fd = create_corfile(data, file)) < 0)
		return (ERR_OPEN);
	if (data->e->flag & FLAG_N)
		print_head(data);
	way_to_corewar(data, data->ao);
	return (0);
}

static void		init_data(t_data *data, t_env *e)
{
	data->e = e;
	data->ao = e->aolist;
}

int				dump_to_file(t_env *e)
{
	t_data	data;

	ft_memset(&data, 0, sizeof(t_data));
	if (e->flag & FLAG_N)
		init_screen(&data);
	init_data(&data, e);
	cw_hexdump(&data, e->fd_name);
	if (e->flag & FLAG_N)
	{
		print_menu(&data);
		print_hex(&data);
		print_end(&data);
		endwin();
	}
	if ((e->flag & FLAG_O) && !(e->flag & FLAG_N))
		print_fsize(&data);
	if ((e->flag & FLAG_P) || (e->flag & FLAG_GP))
		print_path(&data);
	close(data.fd);
	return (0);
}
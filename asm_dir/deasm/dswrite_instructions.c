/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dswrite_instructions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/30 01:14:48 by mmonier           #+#    #+#             */
/*   Updated: 2019/10/01 00:53:55 by mmonier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deasm.h"
#include <stdio.h>

void		dswrite_opc(t_info *inf, char buff)
{
	static char		*tab[17] = {NULL, "live", "ld", "st", "add", "sub", "and", "or",
								"xor", "zjmp", "ldi", "sti", "fork", "lld", "lldi",
								"lfork", "aff"};
	unsigned int	i;
	
	i = (unsigned int)buff;
	if (i >= 1 && i <= 16)
	{
		ft_dprintf(inf->ds_fd, "%s\t", tab[i]);
		if ((i >= 1 && i <= 8) || i == 13 || i == 16)
			inf->dir_size = 4;
		else
			inf->dir_size = 2;
	}
}

static void	check_type(t_info *inf, int j)
{
	if (inf->curr[0] == 1 && inf->curr[1] == 0)
	{
		inf->type[j] = TYPE_REG;
		inf->size[j] = 1;
		inf->n_param = inf->n_param + 1;
	}
	else if (inf->curr[0] == 0 && inf->curr[1] == 1)
	{
		inf->type[j] = TYPE_DIR;
		inf->size[j] = inf->dir_size;
		inf->n_param = inf->n_param + 1;
	}
	else if (inf->curr[0] == 1 && inf->curr[1] == 1)
	{
		inf->type[j] = TYPE_IND;
		inf->size[j] = 2;
		inf->n_param = inf->n_param + 1;
	}
}

void		decode_types(t_info *inf, unsigned int buff)
{
	int i;
	int j;
	int stop;

	i = 0;
	j = 2;
	stop = 7;
	buff >>= 2;
	while (stop-- > 0)
	{
		if (i > 1)
		{
			check_type(inf, j);
			inf->curr[0] = 0;
			inf->curr[1] = 0;
			i = 0;
			j--;
			if (j < 0)
				j = 2;
		}
		inf->curr[i] = 1 & buff;
		i++;
		buff >>= 1;
	}
}

static void		reset_tab(t_info *inf)
{
	inf->type[0] = 0;
	inf->type[1] = 0;
	inf->type[2] = 0;
	inf->size[0] = 0;
	inf->size[1] = 0;
	inf->size[2] = 0;
	inf->n_param = 0;
	inf->i = 0;
	inf->wait = 0;
	inf->bin = 0;
}

void		dswrite_param(t_info *inf, unsigned char buff)
{
	if (inf->size[inf->i] > 1)
	{
		printf("enter : buff = %x\n", buff);
		if (inf->bin == 0x0)
			inf->bin = (int)buff;
		else
		{
			inf->bin <<= 8;
			inf->bin |= (int)buff;
		}
		if (inf->wait == inf->size[inf->i])
		{
			printf("ENTERING IF TO WRITE---------------\n");
			printf("bin = %x\n", inf->bin);
			printf("buff = %x\n", buff);
			printf("inf->wait == %d -- size = %d\n", inf->wait, inf->size[inf->i]);
			getchar();
			if (inf->type[inf->i] == TYPE_DIR)
				ft_dprintf(inf->ds_fd, "%%%d", inf->bin);
			if (inf->type[inf->i] == TYPE_IND)
				ft_dprintf(inf->ds_fd, "%d", inf->bin);
			if (inf->i < inf->n_param - 1)
				ft_dprintf(inf->ds_fd, ", ");
			inf->i = inf->i + 1;
			inf->wait = 0;
			inf->bin = 0;
		}
		inf->wait = inf->wait + 1;
	}
	else
	{
		printf("buff = %x\n", buff);
		ft_dprintf(inf->ds_fd, "r%d", buff); 
		inf->i = inf->i + 1;
	}
	if (inf->i == inf->n_param)
	{
		ft_dprintf(inf->ds_fd, "\n");
		reset_tab(inf);
		inf->decode = OPC;
	}
}
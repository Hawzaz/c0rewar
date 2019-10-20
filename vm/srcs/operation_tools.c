/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:05:26 by bmellon           #+#    #+#             */
/*   Updated: 2019/10/20 20:52:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

static int	get_params_size(t_param *params, t_op *op, uint8_t data_type)
{
	uint8_t	type;
	int		i;

	i = 0;
	if (op->reg_nb > 1 && op->types[0] != T_DIR)
		while (i < op->reg_nb)
		{
			type = data_type;
			type = type << (i * 2);
			params[i].type = type >> 6;
			if (params[i].type == REG_CODE && op->types[i] & T_REG)
				params[i].size = 1;
			else if (params[i].type == DIR_CODE && op->types[i] & T_DIR)
				params[i].size = (op->direct_size) ? 2 : 4;
			else if (params[i].type == IND_CODE && op->types[i] & T_IND)
				params[i].size = 2;
			else
				return (0);
			i++;
			type = type << 2;
		}
	else
		params[i].type = op->types[0];
	return (1);
}

static int	get_param_value(uint8_t *data, int index, int size)
{
	int		i;
	char	tab[REG_SIZE];

	ft_bzero(tab, REG_SIZE);
	i = 0;
	if (size > REG_SIZE)
		return (0);
	while (size--)
	{
		tab[size] = data[(index + i) % MEM_SIZE];
		i++;
	}
	if (size == 2)
		return (*(short *)tab);
	return (*(int *)tab);
}

int			get_params(t_param *params, t_op *op, t_process *proc, void *arena)
{
	REG_CAST	pc;
	uint8_t		*data;
	int			i;

	pc = *(REG_CAST *)proc->pc;
	data = (uint8_t *)arena + (pc + 1) % MEM_SIZE;
	if (!get_params_size(params, op, *data))
		return (0);
	i = 0;
	while (i < op->reg_nb)
	{
		if (op->reg_nb == 1 && op->types[0] == T_DIR)
			params[i].value = get_param_value(arena, pc + 1, params[i].size);
		else
			params[i].value = get_param_value(arena, pc + 2, params[i].size);
		if (!(op->types[i] & params[i].type)
		|| (params[i].type == REG_CODE
			&& (params[i].value < 1 || params[i].value > REG_NUMBER)))
			return (0);
		pc += params[i].size;
		i++;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 18:56:21 by mmonier           #+#    #+#             */
/*   Updated: 2019/11/05 00:29:07 by mmonier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print.h"

static int	getsize(int type)
{
	if (type == E_MAGIC)
		return (3);
	if (type == E_PAD || type == E_SIZE)
		return (4);
	if (type == E_NAME)
		return (128);
	if (type == E_COMMENT)
		return (2048);
	//	return (2048 - (128 + (2 * 4)));
	return (1);
}

static int	gettype(int pos)
{
	if (pos < 4)
		return (E_MAGIC);
	if (pos >= 4 && pos <= 131)
		return (E_NAME);
	if (pos > 131 && pos < 135)
		return (E_PAD);
	if (pos >= 135 && pos < 139)
		return (E_SIZE);
	if (pos >= 139 && pos < 2188)
		return (E_COMMENT);	
	if (pos >= 2188 && pos < 2192)
		return (E_PAD);
	return (E_OPCODE);
}

static void	getvalue(t_print *print, t_val *val)
{
	print->val = val;
	val->type = gettype(print->pos);
	val->size = getsize(val->type);
	printf("val size : [%d]\n", val->size);
}

void		visuprint(t_print *print)
{
	int				ret;
	unsigned char	buff;
	t_val			val;

	ft_memset(&val, 0, sizeof(t_val));
	print->pos = 1;
	while (1)
	{
		if ((ret = read(print->fd, &buff, 1)) <= 0)
			return ;
		printcurrent(print, buff);
	//	printf("val size : [%d]\n", val.size);
		if (print->pos == 4)
			printf("\n\nEND OF MN\n");
		else if (print->pos == 128 + 4)
			printf("\n\n END OF NAME\n");
		else if (print->pos == 136)
			printf("\n\n END OF PADDING\n");
		else if (print->pos == 140)
			printf("\n\nEND OF SIZE\n");
		else if (print->pos == 140 + 2048)
			printf("\n\n END OF COMMENT\n");
		else if (print->pos == 2189)
			printf("\n\n END OF OPCODE\n");
		if (buff == 0x0c)
			break ;
		if (val.size == 0)
		{
			if (print->pos > 2192)
				rendersyntax(print, &val, buff);
			else
				getvalue(print, &val);
		}
		print->pos += 1;
		val.size -= 1;
		//ft_printf("val size : %d\n", val.size);
		//getchar();
	}
}
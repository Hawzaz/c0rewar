/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   champ.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 16:05:42 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/20 01:02:51 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "vm.h"
#include "ft_printf.h"

static uint32_t	byteswap_32(uint32_t x)
{
	return (((x & 0xff000000u) >> 24) | ((x & 0x00ff0000u) >> 8)
		| ((x & 0x0000ff00u) << 8) | ((x & 0x000000ffu) << 24));
}

static int		get_data(int fd, t_process *proc)
{
	ssize_t		ret;
	size_t		off;
	char		buff[1024];

	off = 0;
	while ((ret = read(fd, buff, sizeof(buff))) > 0)
	{
		ft_memcpy((char *)proc->file + off, buff, ret);
		off += ret;
	}
	if (ret)
		return (ERR_READ);
	if (off <= sizeof(t_header))
		return (ERR_SIZE_LOW);
	if (byteswap_32(((t_header *)proc->file)->magic) != COREWAR_EXEC_MAGIC)
		return (ERR_MAGIC);
	off -= sizeof(t_header);
	if (off != byteswap_32(((t_header *)proc->file)->prog_size))
		return (ERR_SIZE_DIFF);
	if (off > CHAMP_MAX_SIZE)
		return (ERR_SIZE_HIGH);
	proc->data_size = off;
	return (IS_OK);
}

int				get_champions(t_env *e)
{
	int			i;
	int			fd;
	off_t		ret;

	i = 0;
	while (i < e->nb_players)
	{
		if ((fd = open(e->proc[i].name, O_RDONLY)) < 0)
			return (corewar_errors(ERR_OPEN, e->proc[i].name, e));
		if ((e->proc[i].file_size = lseek(fd, 0, SEEK_END)) < 0)
			return (corewar_errors(ERR_LSEEK, e->proc[i].name, e));
		if ((e->proc[i].file = malloc(e->proc[i].file_size)) == NULL)
			return (corewar_errors(ERR_MALLOC, e->proc[i].name, e));
		if (lseek(fd, 0, SEEK_SET) < 0)
			return (corewar_errors(ERR_LSEEK, e->proc[i].name, e));
		ret = get_data(fd, &e->proc[i]);
		close(fd);
		if (ret)
			return (corewar_errors(ret, e->proc[i].name, e));
		i++;
	}
	return (IS_OK);
}

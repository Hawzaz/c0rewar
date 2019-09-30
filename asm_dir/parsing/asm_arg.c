/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 21:40:14 by dbaffier          #+#    #+#             */
/*   Updated: 2019/09/30 23:50:09 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "asm.h"

static void		parse_splited(t_token *ptr, char *str)
{
	ptr->err = ft_strdup(str);
	if (str[0] == 'r')
	{
		ptr->lab = ft_strdup(str + 1);
		ptr->type |= REGISTER;
	}
	else if (str[0] == DIRECT_CHAR)
	{
		if (str[1] == LABEL_CHAR)
		{
			ptr->lab = ft_strdup(str + 2);
			ptr->type |= DIRECT | LAB;
		}
		else
		{
			ptr->lab = ft_strdup(str + 1);
			ptr->type |= DIRECT;
		}
	}
	else if (str[0] == LABEL_CHAR)
	{
		ptr->lab = ft_strdup(str + 1);
		ptr->type |= UNDIRECT | LAB;
	}
	else
	{
		ptr->lab = ft_strdup(str);
		ptr->type |= UNDIRECT;
	}
}

static t_token		*parse_arg(t_token *ptr, char **splited, size_t *size)
{
	size_t		i;
	t_token		*save;

	i = 0;
	save = ptr;
	while (splited[i])
	{
		parse_splited(ptr, splited[i]);
		if (splited[i + 1])
		{
			if (!(ptr->next = ft_memalloc(sizeof(t_token))))
				return (NULL);
			ptr = ptr->next;
			*size = *size + 1;
		}
		i++;
	}
	return (ptr);
}

static int		evaluate_sc(char *line, size_t i)
{
	while (line[i])
	{
		if (line[i] == ',' && line[i + 1] == ',')
			return (1);
		i++;
	}
	if (line[i - 1] == ',')
		return (1);
	return (0);
}

int		grep_arg(t_token *ptr, char *line, size_t *i)
{
	size_t		j;
	size_t		size;
	t_token		*save;
	char		**arg;
	char		**splited;

	j = 0;
	size = 0;
	save = ptr;
	if (evaluate_sc(line, *i))
	{
		while (line[*i])
			*i = *i + 1;
		return (0);
	}
	if ((arg = ft_strsplit(&line[*i], ',')) == NULL)
		return (ERR_MALLOC);
	while (arg[j])
	{
		splited = ft_strsplitws(arg[j]);
		if (!(ptr = parse_arg(ptr, splited, &size)))
			return (ERR_MALLOC);
		if (arg[j + 1])
		{
			if (!(ptr->next = ft_memalloc(sizeof(t_token))))
				return (ERR_MALLOC);
			ptr = ptr->next;
		}
		ft_freetab(&splited);
		j++;
		size++;
	}
	save->arg_n = size;
	ft_freetab(&arg);
	while (line[*i])
		*i = *i + 1;
	return (0);
}


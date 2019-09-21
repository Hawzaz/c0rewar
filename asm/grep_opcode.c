/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grep_opcode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/19 23:25:39 by dbaffier          #+#    #+#             */
/*   Updated: 2019/09/22 00:27:03 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "asm.h"

static char		*dup_val(char *line, size_t *i)
{
	char		*new;
	size_t		s;
	size_t		e;

	s = *i;
	e = 0;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t')
		*i = *i + 1;
	if (!(new = malloc(sizeof(char) * (*i - s) + 1)))
		return (NULL);
	while (line[s] && s < *i)
	{
		new[e] = line[s];
		s++;
		e++;
	}
	new[e] = '\0';
	return (new);
}

static char	*dup_opcode(char *line, size_t *i)
{	
	char		*new;
	size_t		s;
	size_t		e;

	s = *i;
	e = 0;
	while (line[*i] && ft_isalnum(line[*i]))
		*i = *i + 1;
	if (!(new = malloc(sizeof(char) * (*i - s) + 1)))
		return (NULL);
	while (line[s] && s < *i)
	{
		new[e] = line[s];
		s++;
		e++;
	}
	new[e] = '\0';
	return (new);
}

int		grep_opcode(t_token **head, char *line, size_t st, size_t *i)
{
	t_token		*new;
	t_token		*curr;

	if (!(new = ft_memalloc(sizeof(t_token))))
		return (ERR_MALLOC);
	if (!(new->val = dup_opcode(line, i)))
		return (ERR_MALLOC);
	new->type = OP_CODE;
	if (!(new->next = ft_memalloc(sizeof(t_token))))
		return (ERR_MALLOC);
	if (grep_arg(new->next, line, i) > 0)
		return (ERR_MALLOC);
	if (*head == NULL)
		*head = new;
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	return (0);
}

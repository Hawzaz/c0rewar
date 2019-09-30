/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_syntax_label.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 19:57:32 by dbaffier          #+#    #+#             */
/*   Updated: 2019/10/01 00:35:52 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "libft.h"


static t_aolist	*brother_link(t_aolist *head, t_token *curr)
{
	t_token		*ptr;

	while (head)
	{
		ptr = head->tok;
		if (ptr && ptr->type & LABEL)
		{
			if (!ft_strcmp(curr->lab, ptr->lab))
				return (head);
		}
		head = head->next;
	}
	return (NULL);
}

static int	lab_val(t_aolist *curr, t_aolist *next)
{
	//printf("%d -- %d \n", curr->mem_addr, next->mem_addr);
	//return (curr->mem_addr - next->mem_addr);
	return (next->mem_addr - curr->mem_addr);
}

int			asm_lexical_label(char *lab)
{
	size_t	count;
	int		i;
	int		j;
	
	i = 0;
	count = 0;
	while (LABEL_CHARS[i])
	{
		j = 0;
		while (lab[j])
		{
			if (lab[j] == LABEL_CHARS[i])
				count++;
			j++;
		}
		i++;
	}
	if (count != ft_strlen(lab))
		return (1);
	return (0);
}

int			asm_syntax_labelled(t_env *e, t_aolist *head)
{
	while (head)
	{
		if (head->tok && head->tok->type & LABEL)
		{
			if (head->tok->lab && !head->tok->lab[0])
				return (syntax_error(e, E_LEXICAL, head->tok->lab, head->line));
			if (asm_lexical_label(head->tok->lab))
				return (syntax_error(e, E_LEXICAL, head->tok->lab, head->line));
		}
		head = head->next;
	}
	return (0);
}

int			asm_syntax_label(t_env *e, t_aolist *head, t_token *curr)
{
	if (asm_lexical_label(curr->lab))
		return (syntax_error(e, E_LEXICAL, curr->err, head->line));
	if (!(head->lab = brother_link(e->aolist, curr)))
		return (syntax_error(e, E_LAB, curr->err, head->line));
	curr->bin = lab_val(head, head->lab);
	return (0);
}
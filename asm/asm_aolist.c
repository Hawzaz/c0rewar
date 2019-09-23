/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_aolist.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 18:51:48 by dbaffier          #+#    #+#             */
/*   Updated: 2019/09/23 00:46:11 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "asm.h"

void	push_front(void *head, void *new, int type)
{
	t_token		*ptrr;
	t_aolist	*ptr;

	ptr = NULL;
	ptrr = NULL;
	if (type == 1)
	{
		ptrr = (t_token *)head;
		while (ptrr->next)
			ptrr = ptrr->next;
		ptrr->next = (t_token *)new;
	}
	else
	{
		ptr = (t_aolist *)head;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = (t_aolist *)new;
	}
}

int		set_id(t_token *head)
{
	int		i;

	i = 0;
	if (head->type == LABEL)
		head = head->next;
	if (head->type != OP_CODE)
		return (-1);
	return (head->val - 1);
	/*while (g_op_tab[i].reg_name != 0)
	{
		if (!ft_strcmp(head->val, g_op_tab[i].reg_name))
			return (i);
		i++;
	}*/
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 19:51:14 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/23 19:51:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmellon <bmellon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 00:25:07 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/09/21 01:03:01 by bmellon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

static int		get_number(char *av, int *value)
{
	int			i;

	i = 0;
	while (av[i] && ft_isdigit(av[i]))
		i++;
	if (!av[i])
	{
		*value = ft_atoi(av);
		if (*value > 0)
			return (IS_OK);
		return (ERR_NEGATIVE);
	}
	return (ERR_DIGIT);
}

int				get_args(char **av, t_env *e)
{
	int		i;
	char	*tmp;
	int		err;

	i = 1;
	err = 0;
	while (av[i])
	{
		if (ft_strequ(av[i], "-dump"))
			err = get_number(av[++i], &e->dump_cycle);
		else if (ft_strequ(av[i], "-n"))
			err = get_number(av[++i], &e->id);
		else if ((tmp = ft_strrchr(av[i], '.')) && ft_strequ(tmp, ".cor"))
			err = get_player(e, av[i]);
		else
			err = (av[i][0] == '-') ? ERR_PARAM : ERR_FILENAME;
		if (err)
			return (corewar_errors(err, av[i], e));
		i++;
	}
	if (!e->nb_players)
		return (corewar_errors(ERR_NO_CHAMP, NULL, e));
	return (IS_OK);
}

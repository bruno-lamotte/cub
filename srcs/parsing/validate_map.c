/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	check_m_map_sym(char *all_sym, char **map, t_index *i, uint32_t map_pos)
{
	size_t	l;
	size_t	c;

	l = 0;
	while (map[l])
	{
		c = 0;
		while (map[l][c])
		{
			if (map[l][c] != ' ' && !ft_isset(map[l][c], all_sym))
			{
				i->line = map_pos + l;
				i->col = c;
				return (i->err = PERR_INVALIDE_SYMBOL, false);
			}
			c++;
		}
		l++;
	}
	return (true);
}

static void	sub_check_player(t_data *d, ssize_t l, ssize_t c, bool *found)
{
	ssize_t	p_idx;

	d->player_row = l;
	d->player_col = c;
	p_idx = -1;
	while (++p_idx < d->player_len)
	{
		if (d->player_defs[p_idx].symbol == d->m_map[l][c])
		{
			d->player_dir = d->player_defs[p_idx].dir;
			break ;
		}
	}
	*found = true;
}

bool	check_player(t_data *d, t_index *i)
{
	ssize_t	l;
	ssize_t	c;
	bool	player_found;

	player_found = false;
	l = -1;
	while (d->m_map[++l])
	{
		c = -1;
		while (d->m_map[l][++c])
		{
			if (player_found && ft_isset(d->m_map[l][c], d->player_ids))
				return (i->line = d->m_map_section_id + l, i->col = c,
					i->err = PERR_TOO_MANY_PLAYER, false);
			if (ft_isset(d->m_map[l][c], d->player_ids))
				sub_check_player(d, l, c, &player_found);
		}
	}
	if (!player_found)
		return (i->line = d->m_map_section_id, i->col = 0,
			i->err = PERR_PLAYER_NOT_FOUND, false);
	return (true);
}

void	free_bool_tab(bool **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

bool	**dup_str_tab_to_bool_tab(char **map, size_t row_len)
{
	bool	**out;
	size_t	i;
	size_t	tmp_len;

	i = 0;
	out = malloc(sizeof(bool *) * (row_len + 1));
	if (!out)
		return (NULL);
	while (i < row_len)
	{
		tmp_len = ft_strlen(map[i]);
		out[i] = malloc(sizeof(bool) * (tmp_len));
		if (!out[i])
			return (free_bool_tab(out), NULL);
		ft_bzero(out[i], sizeof(bool) * (tmp_len));
		i++;
	}
	out[i] = NULL;
	return (out);
}

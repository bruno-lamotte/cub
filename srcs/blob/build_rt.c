/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_rt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static void	sub_build_cell(t_map_soa *soa_map, t_tmp_cell *cell,
				t_data *data, int16_t res)
{
	if (res >= 0)
	{
		soa_map->occ_id[cell->soa_idx] = (uint8_t)res;
		soa_map->flags[cell->soa_idx] |= CELL_HAS_OBJ;
	}
	else
	{
		res = get_block_id(cell->m, data, T_MSTR);
		if (res >= 0)
		{
			soa_map->occ_id[cell->soa_idx] = (uint8_t)res;
			soa_map->flags[cell->soa_idx] |= CELL_HAS_MSTR;
		}
	}
}

static bool	sub_build_cell_blocks(t_map_soa *soa_map, t_tmp_cell *cell,
				t_data *data)
{
	int16_t	res;

	res = get_block_id(cell->m, data, T_WALL);
	if (res >= 0)
	{
		soa_map->block_id[cell->soa_idx] = (uint8_t)res;
		soa_map->flags[cell->soa_idx] |= CELL_HAS_WALL;
		return (true);
	}
	res = get_block_id(cell->m, data, T_AIR);
	if (res >= 0)
	{
		soa_map->block_id[cell->soa_idx] = (uint8_t)res;
		soa_map->flags[cell->soa_idx] |= CELL_HAS_AIR;
		return (true);
	}
	return (false);
}

void	build_cell(t_map_soa *soa_map, t_tmp_cell *cell, t_data *data)
{
	int16_t	res;

	if (cell->m == ' ')
		return ;
	if (sub_build_cell_blocks(soa_map, cell, data))
		return ;
	res = get_block_id(cell->m, data, T_DOOR);
	if (res >= 0)
	{
		soa_map->occ_id[cell->soa_idx] = (uint8_t)res;
		soa_map->flags[cell->soa_idx] |= CELL_HAS_DOOR;
	}
	else
		sub_build_cell(soa_map, cell, data,
			get_block_id(cell->m, data, T_OBJ));
	soa_map->flags[cell->soa_idx] |= CELL_HAS_AIR;
	soa_map->block_id[cell->soa_idx] = 0;
	if (cell->b && get_block_id(cell->b, data, T_AIR) >= 0)
		soa_map->block_id[cell->soa_idx] = (uint8_t)get_block_id(cell->b,
				data, T_AIR);
}

static void	sub_build_rt(t_data *d, t_tmp_cell *c, t_map_soa *m, void *blob)
{
	c->m = d->m_map[c->l][c->c];
	if (d->have_bonus)
		c->b = d->b_map[c->l][c->c];
	build_cell(m, c, d);
	if (m->flags[c->soa_idx] & CELL_HAS_DOOR)
		build_door_rt(d, get_door_rt(blob), c);
	else if (m->flags[c->soa_idx] & CELL_HAS_MSTR)
		build_mstr_rt(d, get_monster_rt(blob), c);
	c->soa_idx++;
	c->c++;
}

void	build_rt(t_data *d, void *blob)
{
	t_tmp_cell		c;
	t_map_soa		m;

	ft_bzero(&c, sizeof(t_tmp_cell));
	m = get_map_soa(blob);
	while (c.l < d->m_height)
	{
		c.c = 0;
		while (c.c < d->m_max_width && d->m_map[c.l][c.c])
			sub_build_rt(d, &c, &m, blob);
		while (c.c < d->m_max_width)
		{
			c.soa_idx++;
			c.c++;
		}
		c.l++;
	}
}

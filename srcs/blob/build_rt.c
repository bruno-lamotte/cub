#include "../../includes/cub.h"

void build_cell(t_map_soa *soa_map, t_tmp_cell *cell, t_data *data)
{
	int16_t res;

	if (cell->m == ' ')
		return ;
	res = get_block_id(cell->m, data, T_WALL);
	if (res >= 0)
	{
		soa_map->block_id[cell->soa_idx] = (uint8_t)res;
		soa_map->flags[cell->soa_idx] |= CELL_HAS_WALL;
		return ;
	}
	else
	{
		res = get_block_id(cell->m, data, T_AIR);
		if (res >= 0)
		{
			soa_map->block_id[cell->soa_idx] = (uint8_t)res;
			soa_map->flags[cell->soa_idx] |= CELL_HAS_AIR;
			return ;
		}
	}
	res = get_block_id(cell->m, data, T_DOOR);
	if (res >= 0)
	{
		soa_map->occ_id[cell->soa_idx] = (uint8_t)res;
		soa_map->flags[cell->soa_idx] |= CELL_HAS_DOOR;
	}
	else
	{
		res = get_block_id(cell->m, data, T_OBJ);
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
	soa_map->flags[cell->soa_idx] |= CELL_HAS_AIR;
	soa_map->block_id[cell->soa_idx] = 0;
	if (cell->b)
	{
		res = get_block_id(cell->b, data, T_AIR);
		if (res >= 0)
			soa_map->block_id[cell->soa_idx] = (uint8_t)res;
	}
}

/*
typedef struct	s_door_rt
{
	uint8_t		flags;
	uint8_t		def_id;
	uint8_t		open_ratio_255;
	uint8_t		pad;
	uint32_t	map_id;
}				t_door_rt;
*/

void build_door_rt(t_data *data, t_door_rt *door, t_tmp_cell *c)
{
	t_door_def *def;

	door[c->door_idx].def_id = get_def_id(data->door_ids, c->m);
	def = &data->doors_defs[door[c->door_idx].def_id];
	if (!def->is_closed)
		door[c->door_idx].open_ratio_255 = UINT8_MAX;
	door[c->door_idx].map_id = c->soa_idx;
	(c->door_idx)++;
}

/*
typedef struct	s_monster_rt
{
	uint8_t		flags;
	uint8_t		def_id;
	uint8_t		current_hp;
	uint8_t		pad1;
	t_xy_double	pos;
	t_xy_double	dir;
	t_xy_double	last_player_pos;
	uint64_t	pad2;
}				t_monster_rt;
*/

void build_mstr_rt(t_data *data, t_monster_rt *mstr, t_tmp_cell *c)
{
	t_monster_def *def;

	mstr[c->mstr_idx].def_id = get_def_id(data->monster_ids, c->m);
	def = &data->monsters_defs[mstr[c->mstr_idx].def_id];
	mstr[c->mstr_idx].current_hp = def->hp;
	mstr[c->mstr_idx].pos.x = c->c + 0.5f;
	mstr[c->mstr_idx].pos.y = c->l + 0.5f;
	mstr[c->mstr_idx].map_id = c->soa_idx;
	// no dir vec decided yet
	(c->mstr_idx)++;
}

void build_rt(t_data *d, void *blob)
{
	t_tmp_cell	c;
	t_map_soa m;
	t_door_rt *door;
	t_monster_rt *mstr;

	door = get_door_rt(blob);
	mstr = get_monster_rt(blob);
	ft_bzero(&c, sizeof(t_tmp_cell));
	m = get_map_soa(blob);
	while (c.l < d->m_height)
	{
		c.c = 0;
		while (c.c < d->m_max_width && d->m_map[c.l][c.c])
		{
			c.m = d->m_map[c.l][c.c];
			if (d->have_bonus)
				c.b = d->b_map[c.l][c.c];
			build_cell(&m, &c, d);
			if (m.flags[c.soa_idx] & CELL_HAS_DOOR)
				build_door_rt(d, door, &c);
			else if (m.flags[c.soa_idx] & CELL_HAS_MSTR)
				build_mstr_rt(d, mstr, &c);
			c.soa_idx++;
			c.c++;
		}
		while (c.c < d->m_max_width)
		{
			c.soa_idx++;
			c.c++;
		}
		c.l++;
	}
}
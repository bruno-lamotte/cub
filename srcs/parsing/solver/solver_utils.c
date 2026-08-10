#include "cub.h"

int	is_monster_at_cell(t_engine *eng, t_vec2 cell)
{
	t_monster_rt	*mstr;
	int				i;

	mstr = get_monster_rt(eng->blob);
	i = 0;
	while (i < eng->data->monster_rt_count)
	{
		if (!(mstr[i].flags & MONSTER_DEAD))
		{
			if ((int)mstr[i].pos.d.x == cell.i.x
				&& (int)mstr[i].pos.d.y == cell.i.y)
				return (1);
		}
		i++;
	}
	return (0);
}

int	is_door_blocked(t_engine *eng, t_vec2 cell)
{
	uint8_t		*flags;
	t_door_rt	*doors;
	int			w;
	uint32_t	i;

	flags = get_map_flags(eng->blob);
	w = get_map_width(get_blob_hdr(eng->blob));
	if (!(flags[cell.i.y * w + cell.i.x] & CELL_HAS_DOOR))
		return (0);
	doors = get_door_rt(eng->blob);
	i = 0;
	while (i < get_blob_hdr(eng->blob)->door_rt.count)
	{
		if (doors[i].map_id == (uint32_t)(cell.i.y * w + cell.i.x))
			return (doors[i].flags & DOOR_BLOCKED);
		i++;
	}
	return (0);
}

int	is_tile_passable(t_engine *eng, t_vec2 cell)
{
	uint8_t	*flags;
	int		w;
	int		h;

	w = get_map_width(get_blob_hdr(eng->blob));
	h = get_map_height(get_blob_hdr(eng->blob));
	flags = get_map_flags(eng->blob);
	if (cell.i.x < 0 || cell.i.x >= w || cell.i.y < 0 || cell.i.y >= h)
		return (0);
	if (flags[cell.i.y * w + cell.i.x] & CELL_HAS_WALL)
		return (0);
	if (is_door_blocked(eng, cell))
		return (0);
	return (1);
}

t_vec2	find_walkable_neighbor(t_engine *eng, t_vec2 target)
{
	int		dx[4] = {0, 0, -1, 1};
	int		dy[4] = {-1, 1, 0, 0};
	t_vec2	grid;
	t_vec2	res;
	int		d;

	res.i.x = -1;
	res.i.y = -1;
	d = 0;
	while (d < 4)
	{
		grid.i.x = target.i.x + dx[d];
		grid.i.y = target.i.y + dy[d];
		if (grid.i.x >= 0 && grid.i.x < get_map_width(get_blob_hdr(eng->blob))
			&& grid.i.y >= 0 && grid.i.y < get_map_height(get_blob_hdr(eng->blob)))
		{
			if (is_tile_passable(eng, grid))
				return (grid);
		}
		d++;
	}
	return (res);
}

int	is_adjacent_to_terminal(t_engine *eng, t_vec2 *term_pos)
{
	int		dx[4] = {0, 0, -1, 1};
	int		dy[4] = {-1, 1, 0, 0};
	t_vec2	grid;
	int		w;
	int		d;

	w = get_map_width(get_blob_hdr(eng->blob));
	d = -1;
	while (++d < 4)
	{
		grid.i.x = (int)eng->player->pos.d.x + dx[d];
		grid.i.y = (int)eng->player->pos.d.y + dy[d];
		if (grid.i.x >= 0 && grid.i.x < w && grid.i.y >= 0
			&& grid.i.y < get_map_height(get_blob_hdr(eng->blob)))
		{
			if ((get_map_flags(eng->blob)[grid.i.y * w + grid.i.x] & CELL_HAS_OBJ)
				&& eng->data->obj_defs[get_map_occ_ids(eng->blob)[grid.i.y * w
				+ grid.i.x]].symbol == 'T')
			{
				if (term_pos)
					*term_pos = grid;
				return (1);
			}
		}
	}
	return (0);
}

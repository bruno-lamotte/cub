#include "cub.h"
#include <math.h>

double	get_sprite_coords(t_engine *eng, t_vec2 pos,
			t_vec2 *sp, t_vec2 *trans)
{
	double	det;

	sp->d.x = pos.d.x - eng->player->pos.d.x;
	sp->d.y = pos.d.y - eng->player->pos.d.y;
	det = 1.0 / (eng->player->plane.d.x * eng->player->dir.d.y
			- eng->player->dir.d.x * eng->player->plane.d.y);
	trans->d.x = det * (eng->player->dir.d.y * sp->d.x
			- eng->player->dir.d.x * sp->d.y);
	trans->d.y = det * (-eng->player->plane.d.y * sp->d.x
			+ eng->player->plane.d.x * sp->d.y);
	return (trans->d.y);
}

int	collect_monsters(t_engine *eng, t_sprite *sprites, int count)
{
	t_monster_rt	*m;
	t_vec2			sp;
	t_vec2			trans;
	int				i;

	m = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(m[i].flags & MONSTER_DEAD) && count < 512)
		{
			sprites[count].pos = m[i].pos;
			sprites[count].dist = get_sprite_coords(eng, m[i].pos,
					&sp, &trans);
			sprites[count].is_monster = 1;
			sprites[count].m_idx = i;
			count++;
		}
	}
	return (count);
}

int	fill_obj_sprite(t_engine *eng, t_sprite *sprites, int count,
		t_vec2 grid)
{
	t_vec2	sp;
	t_vec2	trans;
	t_vec2	pos;
	char	sym;
	int		idx;

	idx = get_map_occ_ids(eng->blob)[grid.i.y
		* get_map_width(get_blob_hdr(eng->blob)) + grid.i.x];
	sym = eng->data->obj_defs[idx].symbol;
	pos.d.x = grid.i.x + 0.5;
	pos.d.y = grid.i.y + 0.5;
	sprites[count].pos = pos;
	sprites[count].dist = get_sprite_coords(eng, pos, &sp, &trans);
	sprites[count].is_monster = 0;
	sprites[count].sym = sym;
	sprites[count].grid = grid;
	return (count + 1);
}

int	collect_objects(t_engine *eng, t_sprite *sprites, int count)
{
	int		w;
	int		h;
	t_vec2	grid;

	w = get_map_width(get_blob_hdr(eng->blob));
	h = get_map_height(get_blob_hdr(eng->blob));
	grid.i.y = -1;
	while (++grid.i.y < h)
	{
		grid.i.x = -1;
		while (++grid.i.x < w)
		{
			if ((get_map_flags(eng->blob)[grid.i.y * w + grid.i.x]
				& CELL_HAS_OBJ) && count < 512)
			{
				count = fill_obj_sprite(eng, sprites, count, grid);
			}
		}
	}
	return (count);
}

void	sort_sprites(t_sprite *sprites, int count)
{
	int			i;
	int			j;
	t_sprite	temp;

	i = -1;
	while (++i < count - 1)
	{
		j = i;
		while (++j < count)
		{
			if (sprites[i].dist < sprites[j].dist)
			{
				temp = sprites[i];
				sprites[i] = sprites[j];
				sprites[j] = temp;
			}
		}
	}
}

#include "cub.h"
#include <math.h>

double	get_sprite_coords(t_engine *eng, double sx, double sy,
					double sp[2], double trans[2])
{
	double	det;

	sp[0] = sx - eng->player->pos.d.x;
	sp[1] = sy - eng->player->pos.d.y;
	det = 1.0 / (eng->player->plane.d.x * eng->player->dir.d.y
			- eng->player->dir.d.x * eng->player->plane.d.y);
	trans[0] = det * (eng->player->dir.d.y * sp[0] - eng->player->dir.d.x * sp[1]);
	trans[1] = det * (-eng->player->plane.d.y * sp[0] + eng->player->plane.d.x
			* sp[1]);
	return (trans[1]);
}

int	collect_monsters(t_engine *eng, t_sprite *sprites, int count)
{
	t_monster_rt	*m;
	double			sp[2];
	double			trans[2];
	int				i;

	m = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(m[i].flags & MONSTER_DEAD) && count < 512)
		{
			sprites[count].pos.d.x = m[i].pos.d.x;
			sprites[count].pos.d.y = m[i].pos.d.y;
			sprites[count].dist = get_sprite_coords(eng, m[i].pos.d.x,
					m[i].pos.d.y, sp, trans);
			sprites[count].is_monster = 1;
			sprites[count].m_idx = i;
			count++;
		}
	}
	return (count);
}

int	fill_obj_sprite(t_engine *eng, t_sprite *sprites, int count,
				int x, int y)
{
	double	sp[2];
	double	trans[2];
	char	sym;

	sym = eng->data->obj_defs[get_map_occ_ids(eng->blob)[y
			* get_map_width(get_blob_hdr(eng->blob)) + x]].symbol;
	if (sym == 'L')
		return (count);
	sprites[count].pos.d.x = x + 0.5;
	sprites[count].pos.d.y = y + 0.5;
	sprites[count].dist = get_sprite_coords(eng, x + 0.5, y + 0.5, sp, trans);
	sprites[count].is_monster = 0;
	sprites[count].sym = sym;
	sprites[count].grid.i.x = x;
	sprites[count].grid.i.y = y;
	return (count + 1);
}

int	collect_objects(t_engine *eng, t_sprite *sprites, int count)
{
	int		w;
	int		h;
	int		x;
	int		y;

	w = get_map_width(get_blob_hdr(eng->blob));
	h = get_map_height(get_blob_hdr(eng->blob));
	y = -1;
	while (++y < h)
	{
		x = -1;
		while (++x < w)
		{
			if ((get_map_flags(eng->blob)[y * w + x] & CELL_HAS_OBJ)
				&& count < 512)
			{
				count = fill_obj_sprite(eng, sprites, count, x, y);
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

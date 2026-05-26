#include "cub.h"
#include <stdlib.h>
#include <math.h>

static int	sh_idx(double t)
{
	int	idx;

	idx = (int)(t * 10.0f);
	if (idx < 0)
		return (0);
	if (idx >= DIST_MAX)
		return (DIST_MAX - 1);
	return (idx);
}

typedef struct s_sprite
{
	double	x;
	double	y;
	double	dist;
	int		is_monster;
	int		m_idx;
	char	sym;
	int		ox;
	int		oy;
}				t_sprite;

static double	get_sprite_coords(t_engine *eng, double sx, double sy,
					double sp[2], double trans[2])
{
	double	det;

	sp[0] = sx - eng->player->pos.x;
	sp[1] = sy - eng->player->pos.y;
	det = 1.0 / (eng->player->plane.x * eng->player->dir.y
			- eng->player->dir.x * eng->player->plane.y);
	trans[0] = det * (eng->player->dir.y * sp[0] - eng->player->dir.x * sp[1]);
	trans[1] = det * (-eng->player->plane.y * sp[0] + eng->player->plane.x
			* sp[1]);
	return (trans[1]);
}

static void	draw_monster_stripe(t_engine *eng, int stripe, int draw_start_y,
				int draw_end_y, float shade)
{
	int				y;
	unsigned int	col;
	int				r;
	int				g;
	int				b;

	y = draw_start_y - 1;
	while (++y <= draw_end_y)
	{
		col = 0xAA00FF;
		if (y >= draw_start_y + (draw_end_y - draw_start_y) * 0.25
			&& y <= draw_start_y + (draw_end_y - draw_start_y) * 0.35)
			col = 0xFF0000;
		r = (int)(((col >> 16) & 0xFF) * shade);
		g = (int)(((col >> 8) & 0xFF) * shade);
		b = (int)((col & 0xFF) * shade);
		put_pixel(&eng->screen->img2, stripe, y, (r << 16) | (g << 8) | b);
	}
}

static int	collect_monsters(t_engine *eng, t_sprite *sprites, int count)
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
			sprites[count].x = m[i].pos.x;
			sprites[count].y = m[i].pos.y;
			sprites[count].dist = get_sprite_coords(eng, m[i].pos.x,
					m[i].pos.y, sp, trans);
			sprites[count].is_monster = 1;
			sprites[count].m_idx = i;
			count++;
		}
	}
	return (count);
}

static int	fill_obj_sprite(t_engine *eng, t_sprite *sprites, int count,
				int x, int y)
{
	double	sp[2];
	double	trans[2];
	char	sym;

	sym = eng->data->obj_defs[get_map_occ_ids(eng->blob)[y
			* get_map_width(get_blob_hdr(eng->blob)) + x]].symbol;
	if (sym == 'L')
		return (count);
	sprites[count].x = x + 0.5;
	sprites[count].y = y + 0.5;
	sprites[count].dist = get_sprite_coords(eng, x + 0.5, y + 0.5, sp, trans);
	sprites[count].is_monster = 0;
	sprites[count].sym = sym;
	sprites[count].ox = x;
	sprites[count].oy = y;
	return (count + 1);
}

static int	collect_objects(t_engine *eng, t_sprite *sprites, int count)
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

static void	sort_sprites(t_sprite *sprites, int count)
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

static void	draw_object_stripe(t_engine *eng, int stripe, int y_start,
				int y_end, float shade, char sym)
{
	int				y;
	unsigned int	col;
	int				r;
	int				g;
	int				b;

	y = y_start - 1;
	while (++y <= y_end)
	{
		if (sym == 'T')
		{
			col = 0x00FF00;
			if (y >= y_start + (y_end - y_start) * 0.3
				&& y <= y_start + (y_end - y_start) * 0.6)
				col = 0x00FFFF;
		}
		else
		{
			if (eng->alarm_triggered && (eng->pool.current_frame / 10) % 2)
				col = 0xFF0000;
			else if (eng->alarm_triggered)
				col = 0x550000;
			else
				col = 0xCCCCCC;
		}
		r = (int)(((col >> 16) & 0xFF) * shade);
		g = (int)(((col >> 8) & 0xFF) * shade);
		b = (int)((col & 0xFF) * shade);
		put_pixel(&eng->screen->img2, stripe, y, (r << 16) | (g << 8) | b);
	}
}

static void	calc_object_projection(double trans[2], int w_h[5])
{
	w_h[0] = abs((int)(WINDOW_HEIGHT / trans[1]));
	w_h[1] = -w_h[0] / 2 + (int)((WINDOW_WIDTH / 2) * (1.0 + trans[0]
				/ trans[1]));
	w_h[1] = (w_h[1] < 0) ? 0 : w_h[1];
	w_h[2] = w_h[0] / 2 + (int)((WINDOW_WIDTH / 2) * (1.0 + trans[0]
				/ trans[1]));
	w_h[2] = (w_h[2] >= WINDOW_WIDTH) ? WINDOW_WIDTH - 1 : w_h[2];
	w_h[3] = -w_h[0] / 2 + WINDOW_HEIGHT / 2;
	w_h[3] = (w_h[3] < 0) ? 0 : w_h[3];
	w_h[4] = w_h[0] / 2 + WINDOW_HEIGHT / 2;
	w_h[4] = (w_h[4] >= WINDOW_HEIGHT) ? WINDOW_HEIGHT - 1 : w_h[4];
}

static void	render_sprite(t_engine *eng, t_sprite *s, t_lut *lut)
{
	double	sp[2];
	double	trans[2];
	int		w_h[5];
	int		stripe;

	if (get_sprite_coords(eng, s->x, s->y, sp, trans) <= 0.1)
		return ;
	calc_object_projection(trans, w_h);
	stripe = w_h[1] - 1;
	while (++stripe <= w_h[2])
	{
		if (trans[1] < FP_TO_FLOAT(eng->z_buffer[stripe].perp_wall_dist))
		{
			if (s->is_monster)
				draw_monster_stripe(eng, stripe, w_h[3], w_h[4],
					lut->shade_table[sh_idx(trans[1])]);
			else
				draw_object_stripe(eng, stripe, w_h[3], w_h[4],
					lut->shade_table[sh_idx(trans[1])], s->sym);
		}
	}
}

void	draw_monsters_3d(t_engine *eng)
{
	t_sprite	sprites[512];
	t_lut		*lut;
	int			count;
	int			i;

	count = 0;
	count = collect_monsters(eng, sprites, count);
	count = collect_objects(eng, sprites, count);
	sort_sprites(sprites, count);
	lut = get_lut_bdef(eng->blob);
	i = -1;
	while (++i < count)
		render_sprite(eng, &sprites[i], lut);
}

void	draw_objects_3d(t_engine *eng)
{
	(void)eng;
}

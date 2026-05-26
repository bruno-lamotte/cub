#include "cub.h"

# define MINIMAP_DEFAULT_SCALE 0.0667
# define MINIMAP_BORDER_PAD 6

static double	get_minimap_scale(t_engine *eng, int radius)
{
	double	scale;
	int		w;
	int		h;
	double	dx;
	double	dy;
	double	required;

	scale = MINIMAP_DEFAULT_SCALE;
	if (!eng->terminal_mode)
		return (scale);
	w = get_map_width(get_blob_hdr(eng->blob));
	h = get_map_height(get_blob_hdr(eng->blob));
	dx = fmax(eng->player->pos.x, (double)w - eng->player->pos.x);
	dy = fmax(eng->player->pos.y, (double)h - eng->player->pos.y);
	required = fmax(dx, dy) / (double)(radius - MINIMAP_BORDER_PAD);
	if (required > scale)
		scale = required;
	return (scale);
}

static unsigned int	get_obj_color(t_engine *eng, int idx)
{
	if (eng->data->obj_defs[get_map_occ_ids(eng->blob)[idx]].symbol == 'T')
		return (0x00FF00);
	return (eng->alarm_triggered ? 0xFF0000 : 0xFFFF00);
}

static unsigned int	get_door_cell_color(t_engine *eng, t_vec2_i c,
	double g[2], int w)
{
	double	ratio;
	double	frac[2];

	ratio = get_door_ratio(c.x, c.y, w, eng->blob);
	frac[0] = g[0] - c.x;
	frac[1] = g[1] - c.y;
	if (is_door_horiz(c.x, c.y, w, eng->blob))
	{
		if (frac[1] >= 0.35 && frac[1] <= 0.65 && frac[0] >= ratio)
			return (0x784212);
		return (0x1E272C);
	}
	if (frac[0] >= 0.35 && frac[0] <= 0.65 && frac[1] >= ratio)
		return (0x784212);
	return (0x1E272C);
}

static unsigned int	get_cell_color(t_engine *eng, t_vec2_i c, double g[2])
{
	int		w;
	uint8_t	*flg;

	w = get_map_width(get_blob_hdr(eng->blob));
	flg = get_map_flags(eng->blob);
	if (c.x < 0 || c.x >= w || c.y < 0
		|| c.y >= get_map_height(get_blob_hdr(eng->blob)))
		return (0x000000);
	if (flg[c.y * w + c.x] & CELL_HAS_WALL)
		return (0x4A6984);
	if (flg[c.y * w + c.x] & CELL_HAS_OBJ)
		return (get_obj_color(eng, c.y * w + c.x));
	if (flg[c.y * w + c.x] & CELL_HAS_DOOR)
		return (get_door_cell_color(eng, c, g, w));
	return (0x1E272C);
}

static void	draw_minimap_cell(t_engine *eng, t_vec2_i pos, t_vec2_i d,
				int radius, double scale)
{
	int				d2;
	double			g[2];
	unsigned int	col;
	float			sh;
	int				idx;
	float			dist;
	unsigned int	*bg;

	d2 = d.x * d.x + d.y * d.y;
	if (d2 > radius * radius)
		return ;
	if (d2 >= (radius - 3) * (radius - 3))
		return (put_pixel(&eng->screen->img2, pos.x, pos.y, 0x00F0FF));
	g[0] = eng->player->pos.x + d.x * scale;
	g[1] = eng->player->pos.y + d.y * scale;
	idx = 0;
	if (d2 > 0)
	{
		dist = d2 * fast_inv_sqrt((float)d2);
		idx = (int)(dist * (float)(scale * 10.0));
	}
	if (idx >= DIST_MAX)
		idx = DIST_MAX - 1;
	sh = get_lut_bdef(eng->blob)->shade_table[idx];
	col = get_cell_color(eng, (t_vec2_i){(int)g[0] - (g[0] < 0.0),
		(int)g[1] - (g[1] < 0.0)}, g);
	col = (((int)(((col >> 16) & 0xFF) * sh)) << 16)
		| (((int)(((col >> 8) & 0xFF) * sh)) << 8) | ((int)((col & 0xFF) * sh));
	bg = (unsigned int *)(eng->screen->img2.addr + pos.y
			* eng->screen->img2.line_len + pos.x
			* (eng->screen->img2.bpp / 8));
	put_pixel(&eng->screen->img2, pos.x, pos.y, blend_colors(col, *bg, 200));
}

void	draw_minimap(t_engine *eng)
{
	int			radius;
	t_vec2_i	c;
	t_vec2_i	pos;
	t_vec2_i	d;
	double		scale;

	radius = 90;
	scale = get_minimap_scale(eng, radius);
	c.x = WINDOW_WIDTH - radius - 30;
	c.y = radius + 30;
	pos.y = c.y - radius - 1;
	while (++pos.y <= c.y + radius)
	{
		pos.x = c.x - radius - 1;
		while (++pos.x <= c.x + radius)
		{
			d.x = pos.x - c.x;
			d.y = pos.y - c.y;
			if (pos.x >= 0 && pos.x < WINDOW_WIDTH && pos.y >= 0
				&& pos.y < WINDOW_HEIGHT)
				draw_minimap_cell(eng, pos, d, radius, scale);
		}
	}
	draw_minimap_monsters(eng, c, radius, 1.0 / scale);
	draw_minimap_player(eng, c);
}

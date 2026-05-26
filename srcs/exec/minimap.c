#include "cub.h"

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
	int radius)
{
	int				d2;
	double			g[2];
	unsigned int	col;
	float			sh;
	int				idx;
	unsigned int	*bg;

	d2 = d.x * d.x + d.y * d.y;
	if (d2 > radius * radius)
		return ;
	if (d2 >= (radius - 3) * (radius - 3))
		return (put_pixel(&eng->screen->img2, pos.x, pos.y, 0x00F0FF));
	g[0] = eng->player->pos.x + d.x * 0.0667;
	g[1] = eng->player->pos.y + d.y * 0.0667;
	idx = 0;
	if (d2 > 0)
		idx = (int)((d2 * fast_inv_sqrt((float)d2)) * 0.6667f);
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

	radius = 90;
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
				draw_minimap_cell(eng, pos, d, radius);
		}
	}
	draw_minimap_monsters(eng, c, radius);
	draw_minimap_player(eng, c);
}

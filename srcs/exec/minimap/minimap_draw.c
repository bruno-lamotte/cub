#include "cub.h"

double			get_minimap_scale(t_engine *eng, int radius);
unsigned int	get_cell_color(t_engine *eng, t_vec2 c, double g[2]);

static unsigned int	shade_color(unsigned int col, float sh)
{
	int	r;
	int	g;
	int	b;

	r = (int)(((col >> 16) & 0xFF) * sh);
	g = (int)(((col >> 8) & 0xFF) * sh);
	b = (int)((col & 0xFF) * sh);
	return ((r << 16) | (g << 8) | b);
}

static void	draw_minimap_cell(t_engine *eng, t_vec2 pos, t_vec2 c, double scale)
{
	t_vec2	d;
	int		d2;
	double	g[2];
	int		idx;
	t_vec2	cell;

	d = (t_vec2){.i = {pos.i.x - c.i.x, pos.i.y - c.i.y}};
	d2 = d.i.x * d.i.x + d.i.y * d.i.y;
	if (d2 > 8100)
		return ;
	if (d2 >= 7569)
		return (put_pixel(&eng->screen->img2, pos.i.x, pos.i.y, 0x00F0FF));
	g[0] = eng->player->pos.d.x + d.i.x * scale;
	g[1] = eng->player->pos.d.y + d.i.y * scale;
	idx = 0;
	if (d2 > 0)
		idx = (int)(d2 * fast_inv_sqrt((float)d2) * (float)(scale * 10.0));
	if (idx >= DIST_MAX)
		idx = DIST_MAX - 1;
	cell.i.x = (int)g[0] - (g[0] < 0.0);
	cell.i.y = (int)g[1] - (g[1] < 0.0);
	put_pixel(&eng->screen->img2, pos.i.x, pos.i.y, blend_colors(
		shade_color(get_cell_color(eng, cell, g),
			get_lut_bdef(eng->blob)->shade_table[idx]),
		*(unsigned int *)(eng->screen->img2.addr + pos.i.y
			* eng->screen->img2.line_len + pos.i.x * 4), 200));
}

void	draw_minimap(t_engine *eng)
{
	int			radius;
	t_vec2		c;
	t_vec2		pos;
	double		scale;

	radius = 90;
	scale = get_minimap_scale(eng, radius);
	c.i.x = eng->screen->win_width - radius - 30;
	c.i.y = radius + 30;
	pos.i.y = c.i.y - radius - 1;
	while (++pos.i.y <= c.i.y + radius)
	{
		pos.i.x = c.i.x - radius - 1;
		while (++pos.i.x <= c.i.x + radius)
		{
			if (pos.i.x >= 0 && pos.i.x < eng->screen->win_width && pos.i.y >= 0
				&& pos.i.y < eng->screen->win_height)
				draw_minimap_cell(eng, pos, c, scale);
		}
	}
	draw_minimap_monsters(eng, c, radius, 1.0 / scale);
	draw_minimap_player(eng, c);
}

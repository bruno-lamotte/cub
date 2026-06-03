#include "cub.h"

void	put_pixel(t_img *img, int x, int y, int color)
{
	*(unsigned int *)(img->addr + (y * img->line_len
				+ x * (img->bpp >> 3))) = color;
}

int	clamp_idx(int idx)
{
	if (idx < 0)
		return (0);
	if (idx >= DIST_MAX)
		return (DIST_MAX - 1);
	return (idx);
}

float	clamp_float(float val)
{
	if (val > 1.0f)
		return (1.0f);
	return (val);
}

int	clamp_color(int val)
{
	if (val > 255)
		return (255);
	return (val);
}

void	draw_floor_pixel(t_worker *w, t_floor_row *r, int x, float sh[2])
{
	float	sh_n;
	float	sh_a;
	int		c[3];

	if (r->y <= w->z_buffer[x].draw_end)
		return ;
	sh_n = clamp_float((r->ambient_shade + sh[0]) * GAMMA);
	sh_a = clamp_float(sh[1]);
	c[0] = clamp_color((int)(0x33 * sh_n + 255.0f * sh_a));
	c[1] = clamp_color((int)(0x33 * sh_n));
	c[2] = clamp_color((int)(0x33 * sh_n));
	put_pixel(&w->screen->img, x, r->y, (c[0] << 16) | (c[1] << 8) | c[2]);
}

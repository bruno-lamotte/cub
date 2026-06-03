#include "cub.h"

static void	shift_p(double *p, t_ray_data *ray)
{
	if (ray->side == 0)
	{
		if (FP_TO_FLOAT(ray->ray_dir.fp.x) > 0)
			p[1] += -0.01;
		else
			p[1] += 0.01;
	}
	else
	{
		if (FP_TO_FLOAT(ray->ray_dir.fp.y) > 0)
			p[2] += -0.01;
		else
			p[2] += 0.01;
	}
}

static void	render_ceiling(t_draw *d, t_screen *screen, t_lut *lut, int y)
{
	(void)lut;
	put_pixel(&screen->img, d->x, y, 0x000000);
}

static void	render_wall_pixel(t_draw *d, t_screen *scr, t_ray_data *ray,
				float sh_n, float sh_a, int y)
{
	uint32_t	color;
	int			tex_y;
	int			c[3];

	color = d->tex.color;
	if (d->pixels)
	{
		tex_y = FP_TO_INT(d->tex_pos + (y - d->draw_start) * d->step);
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= d->tex.height)
			tex_y = d->tex.height - 1;
		color = d->pixels[d->tex.width * tex_y + d->tex_x];
	}
	c[0] = (((color >> 16) & 0xFF) >> (ray->side == 1));
	c[1] = (((color >> 8) & 0xFF) >> (ray->side == 1));
	c[2] = ((color & 0xFF) >> (ray->side == 1));
	c[0] = clamp_color((int)((float)c[0] * sh_n + 255.0f * sh_a));
	c[1] = clamp_color((int)((float)c[1] * sh_n));
	c[2] = clamp_color((int)((float)c[2] * sh_n));
	put_pixel(&scr->img, d->x, y, (c[0] << 16) | (c[1] << 8) | c[2]);
}

static void	render_pixel_row(t_draw *d, t_worker *w, t_ray_data *ray,
				t_lut *lut, float sh_n, float sh_a, int y)
{
	if (y < d->draw_start)
		render_ceiling(d, w->screen, lut, y);
	else if (y <= d->draw_end)
		render_wall_pixel(d, w->screen, ray, sh_n, sh_a, y);
}

void	render_pixels(t_draw *d, t_worker *w, t_ray_data *ray,
			t_lut *lut)
{
	int		y;
	float	sh_n;
	float	sh_a;
	double	p[3];

	p[0] = FP_TO_FLOAT(ray->perp_wall_dist);
	p[1] = w->player->pos.d.x + p[0] * FP_TO_FLOAT(ray->ray_dir.fp.x);
	p[2] = w->player->pos.d.y + p[0] * FP_TO_FLOAT(ray->ray_dir.fp.y);
	shift_p(p, ray);
	y = clamp_idx((int)(p[0] * 10.0f));
	sh_a = clamp_float(get_alarm_light_at_point(p[1], p[2], w->blob, w->engine));
	sh_n = clamp_float(((lut->shade_table[y] * 0.2f)
				+ compute_light_at_point(p[1], p[2], w->blob, w->engine)) * GAMMA);
	y = -1;
	while (++y <= d->draw_end)
		render_pixel_row(d, w, ray, lut, sh_n, sh_a, y);
}

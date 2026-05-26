#include "cub.h"

static void	render_ceiling(t_draw *d, t_screen *screen, t_lut *lut, int y)
{
	(void)lut;
	put_pixel(&screen->img, d->x, y, 0x000000);
}

static void	render_floor(t_draw *d, t_worker *w, t_ray_data *ray, int y)
{
	double	p[3];
	float	sh[2];
	int		c[3];
	int		idx;

	p[0] = (float)WINDOW_HEIGHT / (2.0f * y - (float)WINDOW_HEIGHT);
	idx = (int)(p[0] * 10.0f);
	idx = (idx < 0) ? 0 : ((idx >= DIST_MAX) ? DIST_MAX - 1 : idx);
	p[1] = w->player->pos.x + p[0] * FP_TO_FLOAT(ray->ray_dir.x);
	p[2] = w->player->pos.y + p[0] * FP_TO_FLOAT(ray->ray_dir.y);
	sh[1] = get_alarm_light_at_point(p[1], p[2], w->blob, w->engine);
	sh[0] = compute_light_at_point(p[1], p[2], ray, get_lut_bdef(w->blob),
			w->blob, w->engine);
	sh[0] = ((get_lut_bdef(w->blob)->shade_table[idx] * 0.2f) + sh[0]) * GAMMA;
	sh[0] = (sh[0] > 1.0f) ? 1.0f : sh[0];
	sh[1] = (sh[1] > 1.0f) ? 1.0f : sh[1];
	c[0] = (int)(0x33 * sh[0] + 255.0f * sh[1]);
	c[1] = (int)(0x33 * sh[0]);
	c[2] = (int)(0x33 * sh[0]);
	c[0] = (c[0] > 255) ? 255 : c[0];
	c[1] = (c[1] > 255) ? 255 : c[1];
	c[2] = (c[2] > 255) ? 255 : c[2];
	put_pixel(&w->screen->img, d->x, y, (c[0] << 16) | (c[1] << 8) | c[2]);
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
		if (tex_y < 0 || tex_y >= d->tex.height)
			tex_y = (tex_y < 0) ? 0 : d->tex.height - 1;
		color = d->pixels[d->tex.width * tex_y + d->tex_x];
	}
	c[0] = (((color >> 16) & 0xFF) >> (ray->side == 1));
	c[1] = (((color >> 8) & 0xFF) >> (ray->side == 1));
	c[2] = ((color & 0xFF) >> (ray->side == 1));
	c[0] = (int)((float)c[0] * sh_n + 255.0f * sh_a);
	c[1] = (int)((float)c[1] * sh_n);
	c[2] = (int)((float)c[2] * sh_n);
	c[0] = (c[0] > 255) ? 255 : c[0];
	c[1] = (c[1] > 255) ? 255 : c[1];
	c[2] = (c[2] > 255) ? 255 : c[2];
	put_pixel(&scr->img, d->x, y, (c[0] << 16) | (c[1] << 8) | c[2]);
}

static void	render_pixel_row(t_draw *d, t_worker *w, t_ray_data *ray,
				t_lut *lut, float sh_n, float sh_a, int y)
{
	if (y < d->draw_start)
		render_ceiling(d, w->screen, lut, y);
	else if (y > d->draw_end)
		render_floor(d, w, ray, y);
	else
		render_wall_pixel(d, w->screen, ray, sh_n, sh_a, y);
}

void	render_pixels(t_draw *d, t_worker *w, t_ray_data *ray,
			t_lut *lut)
{
	int		y;
	float	sh_n;
	float	sh_a;
	double	p[3];
	float	dyn;

	p[0] = FP_TO_FLOAT(ray->perp_wall_dist);
	p[1] = w->player->pos.x + p[0] * FP_TO_FLOAT(ray->ray_dir.x);
	p[2] = w->player->pos.y + p[0] * FP_TO_FLOAT(ray->ray_dir.y);
	if (ray->side == 0)
		p[1] += (FP_TO_FLOAT(ray->ray_dir.x) > 0) ? -0.01 : 0.01;
	else
		p[2] += (FP_TO_FLOAT(ray->ray_dir.y) > 0) ? -0.01 : 0.01;
	y = (int)(p[0] * 10.0f);
	y = (y < 0) ? 0 : ((y >= DIST_MAX) ? DIST_MAX - 1 : y);
	dyn = compute_light_at_point(p[1], p[2], ray, lut, w->blob, w->engine);
	sh_a = get_alarm_light_at_point(p[1], p[2], w->blob, w->engine);
	sh_n = ((lut->shade_table[y] * 0.2f) + dyn) * GAMMA;
	sh_n = (sh_n > 1.0f) ? 1.0f : sh_n;
	sh_a = (sh_a > 1.0f) ? 1.0f : sh_a;
	y = -1;
	while (++y < WINDOW_HEIGHT)
		render_pixel_row(d, w, ray, lut, sh_n, sh_a, y);
}

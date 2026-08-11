/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	shift_p(double *p, t_ray_data *ray)
{
	if (ray->side == 0)
	{
		if (fp_to_float(ray->ray_dir.fp.x) > 0)
			p[1] += -0.01;
		else
			p[1] += 0.01;
	}
	else
	{
		if (fp_to_float(ray->ray_dir.fp.y) > 0)
			p[2] += -0.01;
		else
			p[2] += 0.01;
	}
}

static void	render_wall_pixel(t_draw *d, t_ray_data *ray,
				float *sh, char *pixel_ptr)
{
	uint32_t	color;
	int			tex_y;
	int			c[3];

	color = d->tex.u_val.color;
	if (d->pixels)
	{
		tex_y = fp_to_int(d->tex_pos);
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= d->tex.height)
			tex_y = d->tex.height - 1;
		color = d->pixels[d->tex.width * tex_y + d->tex_x];
	}
	c[0] = (((color >> 16) & 0xFF) >> (ray->side == 1));
	c[1] = (((color >> 8) & 0xFF) >> (ray->side == 1));
	c[2] = ((color & 0xFF) >> (ray->side == 1));
	c[0] = clamp_color((int)((float)c[0] * sh[0] + 255.0f * sh[1]));
	c[1] = clamp_color((int)((float)c[1] * sh[0]));
	c[2] = clamp_color((int)((float)c[2] * sh[0]));
	*(unsigned int *)pixel_ptr = (c[0] << 16) | (c[1] << 8) | c[2];
	d->tex_pos += d->step;
}

static void	get_shading(t_worker *w, t_ray_data *ray, t_lut *lut, float *sh)
{
	double	p[3];
	int		y;

	p[0] = fp_to_float(ray->perp_wall_dist);
	p[1] = w->player->pos.d.x + p[0] * fp_to_float(ray->ray_dir.fp.x);
	p[2] = w->player->pos.d.y + p[0] * fp_to_float(ray->ray_dir.fp.y);
	shift_p(p, ray);
	y = clamp_idx((int)(p[0] * 10.0f));
	sh[1] = clamp_float(get_alarm_light_at_point(p[1], p[2], w->blob,
				w->engine));
	sh[0] = clamp_float(((lut->shade_table[y] * 0.2f)
				+ compute_light_at_point(p[1], p[2], w->blob,
					w->engine)) * GAMMA);
}

void	render_pixels(t_draw *d, t_worker *w, t_ray_data *ray,
			t_lut *lut)
{
	float	sh[2];
	char	*pixel_ptr;
	int		line_len;
	int		y;

	get_shading(w, ray, lut, sh);
	pixel_ptr = w->screen->img.addr + d->x * (w->screen->img.bpp >> 3);
	line_len = w->screen->img.line_len;
	y = -1;
	while (++y <= d->draw_end)
	{
		if (y < d->draw_start)
			*(unsigned int *)pixel_ptr = 0x000000;
		else
			render_wall_pixel(d, ray, sh, pixel_ptr);
		pixel_ptr += line_len;
	}
}

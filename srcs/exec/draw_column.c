/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_column.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

uint8_t	get_wall_tex_id(t_ray_data *ray, void *blob)
{
	t_bdef_wall	*walls;
	t_bdef_door	*doors;
	uint8_t		*flags;
	int			w;

	flags = get_map_flags(blob);
	w = get_map_width(get_blob_hdr(blob));
	if (ray->map_x < 0 || ray->map_x >= w || ray->map_y < 0
		|| ray->map_y >= get_map_height(get_blob_hdr(blob)))
		return (0);
	if (flags[ray->map_y * w + ray->map_x] & CELL_HAS_DOOR)
	{
		doors = get_door_bdef(blob);
		if (ray->side == 0)
			return (doors[ray->hit_type].tex_side);
		return (doors[ray->hit_type].tex_front);
	}
	walls = get_wall_bdef(blob);
	if (ray->side == 0 && ray->ray_dir.fp.x > 0)
		return (walls[ray->hit_type].tex_west);
	if (ray->side == 0)
		return (walls[ray->hit_type].tex_east);
	if (ray->ray_dir.fp.y > 0)
		return (walls[ray->hit_type].tex_north);
	return (walls[ray->hit_type].tex_south);
}

void	init_projection(t_draw *d, t_ray_data *ray, int win_height)
{
	int64_t	height_fp;

	if (ray->perp_wall_dist <= 0)
		ray->perp_wall_dist = int_to_fp(1);
	height_fp = ((int64_t)int_to_fp(win_height) << FP_SHIFT)
		/ ray->perp_wall_dist;
	height_fp = height_fp >> FP_SHIFT;
	if (height_fp > win_height * 64)
		d->line_height = win_height * 64;
	else if (height_fp <= 0)
		d->line_height = 1;
	else
		d->line_height = (int)height_fp;
	d->draw_start = -d->line_height / 2 + win_height / 2;
	if (d->draw_start < 0)
		d->draw_start = 0;
	d->draw_end = d->line_height / 2 + win_height / 2;
	if (d->draw_end >= win_height)
		d->draw_end = win_height - 1;
}

static void	set_tex_mapping(t_draw *d, t_ray_data *ray, void *blob,
				int win_height)
{
	d->pixels = (uint32_t *)((uint8_t *)blob
			+ ((t_blob_hdr *)blob)->pixels_data.start + d->tex.u_val.offset);
	d->tex_x = fp_to_int(d->tex.width * ray->wall_x);
	if ((ray->side == 0 && ray->ray_dir.fp.x > 0)
		|| (ray->side == 1 && ray->ray_dir.fp.y < 0))
		d->tex_x = d->tex.width - d->tex_x - 1;
	if (d->tex_x < 0)
		d->tex_x = 0;
	if (d->tex_x >= d->tex.width)
		d->tex_x = d->tex.width - 1;
	d->step = int_to_fp(d->tex.height) / d->line_height;
	d->tex_pos = (d->draw_start - win_height / 2 + d->line_height / 2)
		* d->step;
}

static void	init_texture_mapping(t_draw *d, t_ray_data *ray, void *blob,
				int win_height)
{
	uint8_t		tex_id;
	t_bdef_tex	*tex_defs;

	tex_id = get_wall_tex_id(ray, blob);
	tex_defs = get_tex_bdef(blob);
	d->tex = tex_defs[tex_id];
	if (d->tex.width > 0 && d->tex.height > 0)
		set_tex_mapping(d, ray, blob, win_height);
	else
	{
		d->pixels = NULL;
		d->tex_x = 0;
		d->step = 0;
		d->tex_pos = 0;
	}
}

void	draw_column(int x, t_worker *w, t_ray_data *ray)
{
	t_draw	d;
	t_lut	*lut;

	lut = get_lut_bdef(w->blob);
	d.x = x;
	init_projection(&d, ray, w->screen->win_height);
	ray->draw_end = d.draw_end;
	init_texture_mapping(&d, ray, w->blob, w->screen->win_height);
	render_pixels(&d, w, ray, lut);
}

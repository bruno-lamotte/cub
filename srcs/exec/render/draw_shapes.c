/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_shapes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	draw_rect(t_img *img, t_vec2 p, t_vec2 sz, int color);
void	draw_filled_circle(t_img *img, t_vec2 c, int r, int color);

void	draw_circle_outline(t_img *img, t_vec2 c, int r, int color)
{
	int	x;
	int	y;

	y = -r - 1;
	while (++y <= r)
	{
		x = -r - 1;
		while (++x <= r)
		{
			if (x * x + y * y >= (r - 2) * (r - 2) && x * x + y * y <= r * r)
			{
				if (c.i.x + x >= 0 && c.i.x + x < img->width
					&& c.i.y + y >= 0 && c.i.y + y < img->height)
					put_pixel(img, c.i.x + x, c.i.y + y, color);
			}
		}
	}
}

void	draw_single_map_obj(t_engine *eng, t_interact_obj *obj,
			t_vec2 p, int is_sel)
{
	t_door_rt	*doors;
	int			color;

	doors = get_door_rt(eng->blob);
	if (obj->is_door)
	{
		if (doors[obj->idx].flags & DOOR_BLOCKED)
			color = 0xFF3300;
		else
			color = 0x33FF33;
		draw_rect(&eng->screen->img2, (t_vec2){.i = {p.i.x - 5, p.i.y - 5}},
			(t_vec2){.i = {10, 10}}, color);
	}
	else
	{
		if (eng->static_lights[obj->idx].is_triggered)
			color = 0xFF0055;
		else
			color = 0xFFFF00;
		draw_filled_circle(&eng->screen->img2, p, 5, color);
	}
	if (is_sel)
		draw_circle_outline(&eng->screen->img2, p, 12, 0xFFFFFF);
}

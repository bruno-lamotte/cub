/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

#define MINIMAP_DEFAULT_SCALE 0.0667
#define MINIMAP_BORDER_PAD 6

double	get_minimap_scale(t_engine *eng, int radius)
{
	double	scale;
	int		w;
	int		h;
	t_vec2	d;
	double	required;

	scale = MINIMAP_DEFAULT_SCALE;
	if (!eng->terminal_mode)
		return (scale);
	w = get_map_width(get_blob_hdr(eng->blob));
	h = get_map_height(get_blob_hdr(eng->blob));
	d.d.x = fmax(eng->player->pos.d.x, (double)w - eng->player->pos.d.x);
	d.d.y = fmax(eng->player->pos.d.y, (double)h - eng->player->pos.d.y);
	required = fmax(d.d.x, d.d.y) / (double)(radius - MINIMAP_BORDER_PAD);
	if (required > scale)
		scale = required;
	return (scale);
}

unsigned int	get_obj_color(t_engine *eng, int idx)
{
	if (eng->data->obj_defs[get_map_occ_ids(eng->blob)[idx]].symbol == 'T')
		return (0x00FF00);
	if (eng->alarm_triggered)
		return (0xFF0000);
	return (0xFFFF00);
}

unsigned int	get_door_cell_color(t_engine *eng, t_vec2 c,
					double g[2], int w)
{
	double	ratio;
	double	frac[2];

	ratio = get_door_ratio(c.i.x, c.i.y, w, eng->blob);
	frac[0] = g[0] - c.i.x;
	frac[1] = g[1] - c.i.y;
	if (is_door_horiz(c.i.x, c.i.y, w, eng->blob))
	{
		if (frac[1] >= 0.35 && frac[1] <= 0.65 && frac[0] >= ratio)
			return (0x784212);
		return (0x1E272C);
	}
	if (frac[0] >= 0.35 && frac[0] <= 0.65 && frac[1] >= ratio)
		return (0x784212);
	return (0x1E272C);
}

unsigned int	get_cell_color(t_engine *eng, t_vec2 c, double g[2])
{
	int		w;
	uint8_t	*flg;

	w = get_map_width(get_blob_hdr(eng->blob));
	flg = get_map_flags(eng->blob);
	if (c.i.x < 0 || c.i.x >= w || c.i.y < 0
		|| c.i.y >= get_map_height(get_blob_hdr(eng->blob)))
		return (0x000000);
	if (flg[c.i.y * w + c.i.x] & CELL_HAS_WALL)
		return (0x4A6984);
	if (flg[c.i.y * w + c.i.x] & CELL_HAS_OBJ)
		return (get_obj_color(eng, c.i.y * w + c.i.x));
	if (flg[c.i.y * w + c.i.x] & CELL_HAS_DOOR)
		return (get_door_cell_color(eng, c, g, w));
	return (0x1E272C);
}

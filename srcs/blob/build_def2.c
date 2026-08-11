/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_def2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	build_tex_bdef(t_data *d, t_bdef_tex *bdef, uint8_t count)
{
	uint8_t		i;
	uint32_t	off;

	i = 0;
	off = 0;
	while (i < count)
	{
		if (d->textures_defs[i].is_color)
			bdef[i].u_val.color = d->textures_defs[i].color;
		else
		{
			bdef[i].height = d->img_tab[i].height;
			bdef[i].width = d->img_tab[i].width;
			bdef[i].u_val.offset = off;
			off += (bdef[i].height * bdef[i].width * sizeof(uint32_t));
		}
		i++;
	}
}

static void	build_shade_lut(t_lut *lut)
{
	int		i;
	float	distance;
	float	intensity;
	float	max_visible_dist;
	float	factor;

	i = 0;
	max_visible_dist = 7.0f;
	while (i < DIST_MAX)
	{
		distance = (float)i / 10.0f;
		if (distance <= 0.5f)
			intensity = 1.0f;
		else if (distance >= max_visible_dist)
			intensity = 0.0f;
		else
		{
			factor = 1.0f - ((distance - 0.5f) / (max_visible_dist - 0.5f));
			intensity = factor * factor;
		}
		lut->shade_table[i] = intensity;
		i++;
	}
}

void	build_lut_bdef(t_lut *lut)
{
	build_shade_lut(lut);
}

void	build_bdef(t_data *d, void *blob)
{
	build_wall_bdef(d->walls_defs, get_wall_bdef(blob), d->walls_len);
	build_air_bdef(d->airs_defs, get_air_bdef(blob), d->airs_len);
	build_door_bdef(d->doors_defs, get_door_bdef(blob), d->doors_len);
	build_mstr_bdef(d->monsters_defs, get_mstr_bdef(blob), d->monsters_len);
	build_obj_bdef(d->obj_defs, get_obj_bdef(blob), d->obj_len);
	build_tex_bdef(d, get_tex_bdef(blob), d->textures_len);
	build_lut_bdef(get_lut_bdef(blob));
}

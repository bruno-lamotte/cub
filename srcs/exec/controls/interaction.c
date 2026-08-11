/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interaction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	update_global_alarm_state(t_engine *eng)
{
	int	i;

	eng->alarm_triggered = 0;
	i = -1;
	while (++i < eng->static_light_count)
	{
		if (eng->static_lights[i].is_alarm
			&& eng->static_lights[i].is_triggered)
		{
			eng->alarm_triggered = 1;
			break ;
		}
	}
}

static int	is_near_t(t_engine *eng, int x, int y)
{
	t_vec2		d;
	uint8_t		*flags;
	uint8_t		*occ;
	int			w;

	flags = get_map_flags(eng->blob);
	occ = get_map_occ_ids(eng->blob);
	w = get_map_width(get_blob_hdr(eng->blob));
	if (!(flags[y * w + x] & CELL_HAS_OBJ))
		return (0);
	if (eng->data->obj_defs[occ[y * w + x]].symbol != 'T')
		return (0);
	d.d.x = eng->player->pos.d.x - (x + 0.5);
	d.d.y = eng->player->pos.d.y - (y + 0.5);
	return (d.d.x * d.d.x + d.d.y * d.d.y < 2.25);
}

static void	check_proximity(t_engine *eng)
{
	int		x;
	int		y;
	int		w;
	int		h;

	w = get_map_width(get_blob_hdr(eng->blob));
	h = get_map_height(get_blob_hdr(eng->blob));
	y = -1;
	while (++y < h)
	{
		x = -1;
		while (++x < w)
		{
			if (is_near_t(eng, x, y))
			{
				init_terminal_mode(eng);
				eng->keys.e = false;
				return ;
			}
		}
	}
}

void	update_interaction(t_engine *eng)
{
	if (eng->keys.e)
	{
		eng->keys.e = false;
		if (eng->terminal_mode)
			eng->terminal_mode = 0;
		else
			check_proximity(eng);
	}
}

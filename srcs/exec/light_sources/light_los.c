/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_los.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

static void	init_los_steps(t_vec2 m, float d[4], float side[2],
				int step[4])
{
	step[2] = (int)m.d.x;
	step[3] = (int)m.d.y;
	if (d[0] < 0)
	{
		step[0] = -1;
		side[0] = ((float)m.d.x - step[2]) * d[2];
	}
	else
	{
		step[0] = 1;
		side[0] = (step[2] + 1.0f - (float)m.d.x) * d[2];
	}
	if (d[1] < 0)
	{
		step[1] = -1;
		side[1] = ((float)m.d.y - step[3]) * d[3];
	}
	else
	{
		step[1] = 1;
		side[1] = (step[3] + 1.0f - (float)m.d.y) * d[3];
	}
}

static void	init_los_dda(t_vec2 m, t_vec2 p, float d_side[6],
				int step[4])
{
	d_side[0] = (float)(p.d.x - m.d.x);
	d_side[1] = (float)(p.d.y - m.d.y);
	if (d_side[0] == 0.0f)
		d_side[2] = 1e30f;
	else
		d_side[2] = fabsf(1.0f / d_side[0]);
	if (d_side[1] == 0.0f)
		d_side[3] = 1e30f;
	else
		d_side[3] = fabsf(1.0f / d_side[1]);
	init_los_steps(m, d_side, &d_side[4], step);
}

int	check_los(t_vec2 p1, t_vec2 p2, void *blob)
{
	t_map_data	map;
	t_blob_hdr	*hdr;
	int			step[4];
	float		d_side[6];

	hdr = get_blob_hdr(blob);
	map.w = hdr->map.u_data.map_data.width;
	map.h = hdr->map.u_data.map_data.height;
	map.flags = get_map_flags(blob);
	map.doors = get_door_rt(blob);
	map.door_count = hdr->door_rt.u_data.count;
	map.target_x = (int)p2.d.x;
	map.target_y = (int)p2.d.y;
	if (!check_los_shadow(p1, p2))
		return (0);
	init_los_dda(p1, p2, d_side, step);
	return (run_dda(step, d_side, &d_side[4], &map));
}

static int	check_side_los(t_vec2 p1, t_vec2 p2, t_vec2 perp, void *blob)
{
	t_vec2	p1_offset;
	t_vec2	p2_offset;

	p1_offset.d.x = p1.d.x + perp.d.x;
	p1_offset.d.y = p1.d.y + perp.d.y;
	p2_offset.d.x = p2.d.x + perp.d.x;
	p2_offset.d.y = p2.d.y + perp.d.y;
	return (check_los(p1_offset, p2_offset, blob));
}

int	check_thick_los(t_vec2 p1, t_vec2 p2, double radius, void *blob)
{
	t_vec2	perp;
	double	dx;
	double	dy;
	double	len;

	if (!check_los(p1, p2, blob))
		return (0);
	dx = p2.d.x - p1.d.x;
	dy = p2.d.y - p1.d.y;
	len = sqrt(dx * dx + dy * dy);
	if (len < 0.01)
		return (1);
	perp.d.x = (-dy / len) * radius;
	perp.d.y = (dx / len) * radius;
	if (!check_side_los(p1, p2, perp, blob))
		return (0);
	perp.d.x = -perp.d.x;
	perp.d.y = -perp.d.y;
	return (check_side_los(p1, p2, perp, blob));
}

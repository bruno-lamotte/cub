/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_entities2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	init_triangle_points(t_vec2 c, t_vec2 dir, double pt[6])
{
	pt[0] = c.i.x + dir.d.x * 8.0;
	pt[1] = c.i.y + dir.d.y * 8.0;
	pt[2] = c.i.x - dir.d.x * 4.0 - dir.d.y * 3.0;
	pt[3] = c.i.y - dir.d.y * 4.0 + dir.d.x * 3.0;
	pt[4] = c.i.x - dir.d.x * 4.0 + dir.d.y * 3.0;
	pt[5] = c.i.y - dir.d.y * 4.0 - dir.d.x * 3.0;
}

void	draw_minimap_player(t_engine *eng, t_vec2 c)
{
	double		pt[6];
	t_vec2		p;
	double		d[3];

	init_triangle_points(c, eng->player->dir, pt);
	p.i.y = c.i.y - 11;
	while (++p.i.y <= c.i.y + 9)
	{
		p.i.x = c.i.x - 11;
		while (++p.i.x <= c.i.x + 9)
		{
			d[0] = (p.i.x - pt[2]) * (pt[1] - pt[3])
				- (pt[0] - pt[2]) * (p.i.y - pt[3]);
			d[1] = (p.i.x - pt[4]) * (pt[3] - pt[5])
				- (pt[2] - pt[4]) * (p.i.y - pt[5]);
			d[2] = (p.i.x - pt[0]) * (pt[5] - pt[1])
				- (pt[4] - pt[0]) * (p.i.y - pt[1]);
			if ((d[0] >= -0.001 && d[1] >= -0.001 && d[2] >= -0.001)
				|| (d[0] <= 0.001 && d[1] <= 0.001 && d[2] <= 0.001))
				put_pixel(&eng->screen->img2, p.i.x, p.i.y, 0x00FF66);
		}
	}
}

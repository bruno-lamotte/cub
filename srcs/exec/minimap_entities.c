/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_entities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	draw_monster_pixel(t_engine *eng, t_vec2 pos)
{
	put_pixel(&eng->screen->img2, pos.i.x, pos.i.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x - 1, pos.i.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x + 1, pos.i.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x, pos.i.y - 1, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x, pos.i.y + 1, 0xFF0000);
}

static int	get_monster_shade_idx(t_engine *eng, t_monster_rt *m)
{
	float	d2;
	int		idx;

	d2 = (m->pos.d.x - eng->player->pos.d.x)
		* (m->pos.d.x - eng->player->pos.d.x)
		+ (m->pos.d.y - eng->player->pos.d.y)
		* (m->pos.d.y - eng->player->pos.d.y);
	if (d2 <= 0.001f)
		return (0);
	idx = (int)(d2 * fast_inv_sqrt(d2) * 10.0f);
	if (idx < 0)
		return (0);
	if (idx >= DIST_MAX)
		return (DIST_MAX - 1);
	return (idx);
}

static void	draw_single_minimap_monster(t_engine *eng, t_monster_rt *m,
				t_vec2 c, t_vec2 scale)
{
	t_vec2		pos;
	int			rad;
	t_lut		*lut;

	lut = get_lut_bdef(eng->blob);
	if (lut->shade_table[get_monster_shade_idx(eng, m)] <= 0.05)
		return ;
	pos.i.x = c.i.x + (int)((m->pos.d.x - eng->player->pos.d.x) * scale.d.y);
	pos.i.y = c.i.y + (int)((m->pos.d.y - eng->player->pos.d.y) * scale.d.y);
	rad = (int)scale.d.x;
	if ((pos.i.x - c.i.x) * (pos.i.x - c.i.x) + (pos.i.y - c.i.y)
		* (pos.i.y - c.i.y) <= (rad - 5) * (rad - 5))
		draw_monster_pixel(eng, pos);
}

void	draw_minimap_monsters(t_engine *eng, t_vec2 c, t_vec2 scale)
{
	t_monster_rt	*m;
	int				i;

	m = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(m[i].flags & MONSTER_DEAD))
			draw_single_minimap_monster(eng, &m[i], c, scale);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_proj.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

static void	calc_object_projection(t_engine *eng, t_vec2 trans, int *w_h)
{
	w_h[0] = abs((int)(eng->screen->win_height / trans.d.y));
	w_h[1] = -w_h[0] / 2 + (int)((eng->screen->win_width / 2)
			* (1.0 + trans.d.x / trans.d.y));
	w_h[2] = w_h[0] / 2 + (int)((eng->screen->win_width / 2)
			* (1.0 + trans.d.x / trans.d.y));
	w_h[3] = -w_h[0] / 2 + eng->screen->win_height / 2;
	w_h[4] = w_h[0] / 2 + eng->screen->win_height / 2;
}

static inline void	get_draw_bounds(t_engine *eng, t_sprite *s,
						t_vec2 trans, int *whb)
{
	t_vec2	params;
	int		center_x;
	int		scaled_w;

	if (s->is_monster)
	{
		params.i.x = (int)((eng->screen->win_width / 2)
				* (1.0 + trans.d.x / trans.d.y));
		params.i.y = whb[0];
		calc_monster_bounds(eng, s, params, &whb[5]);
		return ;
	}
	center_x = whb[1] + whb[0] / 2;
	scaled_w = whb[0];
	if (s->sym == 'L' || s->sym == 'X')
		scaled_w = whb[0] * 0.3;
	else if (s->sym == 'T')
		scaled_w = whb[0] * 0.6;
	whb[5] = center_x - scaled_w / 2;
	whb[6] = center_x + scaled_w / 2;
	if (whb[5] < 0)
		whb[5] = 0;
	if (whb[6] >= eng->screen->win_width)
		whb[6] = eng->screen->win_width - 1;
}

static inline void	draw_sprite_stripes(t_engine *eng, t_vec2 trans,
						int *whb, t_sprite *s)
{
	int	stripe;

	stripe = whb[5] - 1;
	while (++stripe <= whb[6])
	{
		if (trans.d.y < fp_to_float(eng->z_buffer[stripe].perp_wall_dist))
		{
			if (s->is_monster)
				draw_monster_stripe(eng, stripe, whb, s);
			else
				draw_object_stripe(eng, stripe, whb, s);
		}
	}
}

static void	render_sprite(t_engine *eng, t_sprite *s, t_lut *lut)
{
	t_vec2	sp;
	t_vec2	trans;
	int		whb[7];

	if (get_sprite_coords(eng, s->pos, &sp, &trans) <= 0.1)
		return ;
	calc_object_projection(eng, trans, whb);
	s->shade = lut->shade_table[sh_idx(trans.d.y)];
	get_draw_bounds(eng, s, trans, whb);
	draw_sprite_stripes(eng, trans, whb, s);
}

void	draw_monsters_3d(t_engine *eng)
{
	t_sprite	sprites[512];
	t_lut		*lut;
	int			count;
	int			i;

	count = 0;
	count = collect_monsters(eng, sprites, count);
	count = collect_objects(eng, sprites, count);
	sort_sprites(sprites, count);
	lut = get_lut_bdef(eng->blob);
	i = -1;
	while (++i < count)
		render_sprite(eng, &sprites[i], lut);
}

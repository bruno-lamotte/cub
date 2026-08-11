/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hacking_overlay.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 01:39:07 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

void	draw_rect_blend(t_img *img, t_vec2 p, t_vec2 sz, int color);

void	fill_pct_str(char *str, int pct)
{
	int	i;
	int	bars;

	ft_memcpy(str, "HACK: [", 7);
	bars = (pct * 15) / 100;
	i = -1;
	while (++i < 15)
	{
		if (i < bars)
			str[7 + i] = '=';
		else
			str[7 + i] = '.';
	}
	str[22] = ']';
	str[23] = ' ';
	str[24] = '0' + (pct / 100);
	str[25] = '0' + ((pct / 10) % 10);
	str[26] = '0' + (pct % 10);
	str[27] = '%';
	str[28] = '\0';
}

void	draw_hacking_overlay(t_engine *eng)
{
	int	x;
	int	y;

	if (eng->hacking_timer <= 0)
		return ;
	x = (eng->screen->win_width - 400) / 2;
	y = (eng->screen->win_height - 120) / 2;
	draw_rect_blend(&eng->screen->img2, (t_vec2){.i = {x, y}},
		(t_vec2){.i = {400, 120}}, 0x000000);
}

static int	check_terminal_dist(t_engine *eng, int x, int y)
{
	t_vec2	d;

	d.d.x = eng->player->pos.d.x - (x + 0.5);
	d.d.y = eng->player->pos.d.y - (y + 0.5);
	return (d.d.x * d.d.x + d.d.y * d.d.y < 2.25);
}

int	is_near_terminal(t_engine *eng)
{
	uint8_t	*flags;
	uint8_t	*occ;
	int		x;
	int		y;
	int		w;

	flags = get_map_flags(eng->blob);
	occ = get_map_occ_ids(eng->blob);
	w = get_map_width(get_blob_hdr(eng->blob));
	y = -1;
	while (++y < get_map_height(get_blob_hdr(eng->blob)))
	{
		x = -1;
		while (++x < w)
		{
			if (flags[y * w + x] & CELL_HAS_OBJ
				&& eng->data->obj_defs[occ[y * w + x]].symbol == 'T')
			{
				if (check_terminal_dist(eng, x, y))
					return (1);
			}
		}
	}
	return (0);
}

void	draw_hacking_text(t_engine *eng)
{
	int		x;
	int		y;
	int		pct;
	char	str[32];

	if (eng->hacking_timer <= 0)
		return (draw_hacking_prompt(eng));
	x = (eng->screen->win_width - 400) / 2;
	y = (eng->screen->win_height - 120) / 2;
	pct = (90 - eng->hacking_timer) * 100 / 90;
	fill_pct_str(str, pct);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		x + 30, y + 35, 0x00FF00, ">>> DECRYPTING CYBER SYSTEM LINK <<<");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		x + 30, y + 65, 0x00FF00, str);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		x + 30, y + 95, 0xFF0000, "SYS WARNING: MONSTERS REDIRECTING...");
}

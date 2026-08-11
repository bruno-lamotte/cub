/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hacking_terminal2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	draw_door_info(t_engine *eng, t_interact_obj *obj, int x, int cy);
void	draw_alarm_info(t_engine *eng, t_interact_obj *obj, int x, int cy);

static void	draw_selected_info_text(t_engine *eng, int cx, int cy)
{
	t_interact_obj	*obj;
	int				x;

	x = cx + 250;
	if (eng->interact_obj_count <= 0)
	{
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 60,
			0xFF3300, "NO TARGETS DETECTED");
		return ;
	}
	obj = &eng->interact_objs[eng->selected_obj_idx];
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 100,
		0x00FF33, "--- TARGET METADATA ---");
	if (obj->is_door)
		draw_door_info(eng, obj, x, cy);
	else
		draw_alarm_info(eng, obj, x, cy);
}

void	draw_terminal_hacking_text(t_engine *eng)
{
	int	cx;
	int	cy;

	cx = eng->screen->win_width / 2;
	cy = eng->screen->win_height / 2;
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy - 100, 0x00FF33, "=== TERMINAL DECRYPTION LINK ===");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy - 60, 0x00FF33, "Controls:");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy - 30, 0x00FF33, "  [A / D] or [Arrows] : Select target");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy, 0x00FF33, "  [E] or [Space] : Toggle target status");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy + 30, 0x00FF33, "  [Escape] : Close link");
	draw_selected_info_text(eng, cx, cy);
}

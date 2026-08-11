/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 09:42:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/06/23 09:42:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

static void	rotate_mouse(t_player_rt *p, double speed)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = p->dir.d.x;
	p->dir.d.x = p->dir.d.x * cos(speed) - p->dir.d.y * sin(speed);
	p->dir.d.y = old_dir_x * sin(speed) + p->dir.d.y * cos(speed);
	old_plane_x = p->plane.d.x;
	p->plane.d.x = p->plane.d.x * cos(speed) - p->plane.d.y * sin(speed);
	p->plane.d.y = old_plane_x * sin(speed) + p->plane.d.y * cos(speed);
}

int	mouse_move(int x, int y, t_engine *engine)
{
	static int	old_x = -1;
	int			cx;
	int			cy;

	(void)y;
	if (!engine || !engine->screen || engine->terminal_mode)
	{
		old_x = -1;
		return (0);
	}
	if (old_x == -1)
	{
		old_x = x;
		return (0);
	}
	if (x != old_x)
	{
		rotate_mouse(engine->player, (x - old_x) * 0.002);
		cx = engine->screen->win_width / 2;
		cy = engine->screen->win_height / 2;
		mlx_mouse_move(engine->screen->mlx_ptr,
			engine->screen->win_ptr, cx, cy);
		old_x = cx;
	}
	return (0);
}

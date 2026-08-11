/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 01:39:07 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

#define MOVE_SPEED 0.05
#define ROT_SPEED 0.03

static void	try_move_player(t_engine *eng, t_vec2 d)
{
	double	nx;
	double	ny;

	nx = eng->player->pos.d.x + d.d.x;
	ny = eng->player->pos.d.y + d.d.y;
	if (is_valid_position(nx, eng->player->pos.d.y, eng->blob)
		&& check_player_mstr_collision(nx, eng->player->pos.d.y, eng))
		eng->player->pos.d.x = nx;
	if (is_valid_position(eng->player->pos.d.x, ny, eng->blob)
		&& check_player_mstr_collision(eng->player->pos.d.x, ny, eng))
		eng->player->pos.d.y = ny;
}

static void	add_move_dir(t_vec2 *d, double dx, double dy)
{
	d->d.x += dx * MOVE_SPEED;
	d->d.y += dy * MOVE_SPEED;
}

void	update_position(t_engine *eng, t_keys *keys)
{
	t_vec2		d;
	t_player_rt	*p;

	p = eng->player;
	d.d.x = 0;
	d.d.y = 0;
	if (keys->w)
		add_move_dir(&d, p->dir.d.x, p->dir.d.y);
	if (keys->s)
		add_move_dir(&d, -p->dir.d.x, -p->dir.d.y);
	if (keys->d)
		add_move_dir(&d, -p->dir.d.y, p->dir.d.x);
	if (keys->a)
		add_move_dir(&d, p->dir.d.y, -p->dir.d.x);
	if (d.d.x != 0 || d.d.y != 0)
		try_move_player(eng, d);
}

void	update_rotation(t_player_rt *p, t_keys *keys)
{
	double	old_dir_x;
	double	old_plane_x;
	double	speed;

	if (!keys->left && !keys->right)
		return ;
	speed = ROT_SPEED;
	if (keys->left)
		speed = -ROT_SPEED;
	old_dir_x = p->dir.d.x;
	p->dir.d.x = p->dir.d.x * cos(speed) - p->dir.d.y * sin(speed);
	p->dir.d.y = old_dir_x * sin(speed) + p->dir.d.y * cos(speed);
	old_plane_x = p->plane.d.x;
	p->plane.d.x = p->plane.d.x * cos(speed) - p->plane.d.y * sin(speed);
	p->plane.d.y = old_plane_x * sin(speed) + p->plane.d.y * cos(speed);
}

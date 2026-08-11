/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_def.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	build_wall_bdef(t_wall_def *def, t_bdef_wall *bdef, uint8_t count)
{
	uint8_t	i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex_north = def[i].tex_north;
		bdef[i].tex_west = def[i].tex_west;
		bdef[i].tex_south = def[i].tex_south;
		bdef[i].tex_east = def[i].tex_east;
		i++;
	}
}

void	build_air_bdef(t_air_def *def, t_bdef_air *bdef, uint8_t count)
{
	uint8_t	i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex_ceiling = def[i].tex_ceiling;
		bdef[i].tex_floor = def[i].tex_floor;
		i++;
	}
}

void	build_door_bdef(t_door_def *def, t_bdef_door *bdef, uint8_t count)
{
	uint8_t	i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex_back = def[i].tex_back;
		bdef[i].tex_front = def[i].tex_front;
		bdef[i].tex_side = def[i].tex_side;
		i++;
	}
}

void	build_mstr_bdef(t_monster_def *def, t_bdef_monster *bdef,
			uint8_t count)
{
	uint8_t	i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex = def[i].tex;
		bdef[i].initial_hp = def[i].hp;
		bdef[i].size_pc = def[i].size_pc;
		i++;
	}
}

void	build_obj_bdef(t_obj_def *def, t_bdef_obj *bdef, uint8_t count)
{
	uint8_t	i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex = def[i].tex;
		bdef[i].size_pc = def[i].size_pc;
		i++;
	}
}

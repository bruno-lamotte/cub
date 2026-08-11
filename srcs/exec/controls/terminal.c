/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	trigger_selected_obj(t_engine *eng)
{
	t_interact_obj	*obj;
	t_door_rt		*doors;

	if (eng->interact_obj_count <= 0)
		return ;
	obj = &eng->interact_objs[eng->selected_obj_idx];
	if (obj->is_door)
	{
		doors = get_door_rt(eng->blob);
		doors[obj->idx].flags ^= DOOR_BLOCKED;
	}
	else
	{
		eng->static_lights[obj->idx].is_triggered ^= 1;
		update_global_alarm_state(eng);
	}
}

static void	add_terminal_lights(t_engine *eng)
{
	int	i;

	i = -1;
	while (++i < eng->static_light_count)
	{
		if (eng->interact_obj_count >= 64)
			break ;
		if (eng->static_lights[i].is_alarm)
		{
			eng->interact_objs[eng->interact_obj_count].is_door = 0;
			eng->interact_objs[eng->interact_obj_count].idx = i;
			eng->interact_objs[eng->interact_obj_count].pos.d.x
				= eng->static_lights[i].x;
			eng->interact_objs[eng->interact_obj_count].pos.d.y
				= eng->static_lights[i].y;
			eng->interact_obj_count++;
		}
	}
}

static void	add_terminal_doors(t_engine *eng)
{
	t_door_rt	*doors;
	int			count;
	int			w;
	int			i;

	doors = get_door_rt(eng->blob);
	count = get_blob_hdr(eng->blob)->door_rt.u_data.count;
	w = get_map_width(get_blob_hdr(eng->blob));
	i = -1;
	while (++i < count)
	{
		if (eng->interact_obj_count >= 64)
			break ;
		eng->interact_objs[eng->interact_obj_count].is_door = 1;
		eng->interact_objs[eng->interact_obj_count].idx = i;
		eng->interact_objs[eng->interact_obj_count].pos.d.x
			= (doors[i].map_id % w) + 0.5;
		eng->interact_objs[eng->interact_obj_count].pos.d.y
			= (doors[i].map_id / w) + 0.5;
		eng->interact_obj_count++;
	}
}

void	init_terminal_mode(t_engine *eng)
{
	eng->terminal_mode = 1;
	eng->selected_obj_idx = 0;
	eng->interact_obj_count = 0;
	add_terminal_lights(eng);
	add_terminal_doors(eng);
}

int	handle_terminal_key(int keycode, t_engine *engine)
{
	if (keycode == XK_Escape)
		engine->terminal_mode = 0;
	else if (keycode == XK_Left || keycode == XK_a || keycode == 'a'
		|| keycode == 'A')
	{
		if (engine->interact_obj_count > 0)
			engine->selected_obj_idx = (engine->selected_obj_idx - 1
					+ engine->interact_obj_count) % engine->interact_obj_count;
	}
	else if (keycode == XK_Right || keycode == XK_d || keycode == 'd'
		|| keycode == 'D')
	{
		if (engine->interact_obj_count > 0)
			engine->selected_obj_idx = (engine->selected_obj_idx + 1)
				% engine->interact_obj_count;
	}
	else if (keycode == 'e' || keycode == 'E' || keycode == ' ')
	{
		trigger_selected_obj(engine);
		engine->terminal_mode = 0;
	}
	return (0);
}

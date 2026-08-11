/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_rt2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	build_door_rt(t_data *data, t_door_rt *door, t_tmp_cell *c)
{
	t_door_def	*def;

	door[c->door_idx].def_id = get_def_id(data->door_ids, c->m);
	def = &data->doors_defs[door[c->door_idx].def_id];
	if (!def->is_closed)
		door[c->door_idx].open_ratio_255 = UINT8_MAX;
	door[c->door_idx].map_id = c->soa_idx;
	(c->door_idx)++;
}

void	build_mstr_rt(t_data *data, t_monster_rt *mstr, t_tmp_cell *c)
{
	t_monster_def	*def;

	mstr[c->mstr_idx].def_id = get_def_id(data->monster_ids, c->m);
	def = &data->monsters_defs[mstr[c->mstr_idx].def_id];
	mstr[c->mstr_idx].current_hp = def->hp;
	mstr[c->mstr_idx].pos.d.x = c->c + 0.5f;
	mstr[c->mstr_idx].pos.d.y = c->l + 0.5f;
	mstr[c->mstr_idx].map_id = c->soa_idx;
	(c->mstr_idx)++;
}

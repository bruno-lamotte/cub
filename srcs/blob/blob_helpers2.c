/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blob_helpers2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

uint8_t	*get_map_block_ids(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((uint8_t *)blob + hdr->map.start + hdr->map.used / MAP_SOA_LAYERS);
}

uint8_t	*get_map_occ_ids(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((uint8_t *)blob + hdr->map.start
		+ (hdr->map.used / MAP_SOA_LAYERS) * 2);
}

t_map_soa	get_map_soa(void *blob)
{
	t_map_soa	m;

	m.flags = get_map_flags(blob);
	m.block_id = get_map_block_ids(blob);
	m.occ_id = get_map_occ_ids(blob);
	return (m);
}

t_monster_rt	*get_monster_rt(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_monster_rt *)((uint8_t *)blob + hdr->mstr_rt.start));
}

t_door_rt	*get_door_rt(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_door_rt *)((uint8_t *)blob + hdr->door_rt.start));
}

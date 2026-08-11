/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blob_helpers3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

t_bdef_wall	*get_wall_bdef(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_wall *)((uint8_t *)blob + hdr->wall_def.start));
}

t_bdef_air	*get_air_bdef(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_air *)((uint8_t *)blob + hdr->air_def.start));
}

t_bdef_door	*get_door_bdef(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_door *)((uint8_t *)blob + hdr->door_def.start));
}

t_bdef_monster	*get_mstr_bdef(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_monster *)((uint8_t *)blob + hdr->mstr_def.start));
}

t_bdef_obj	*get_obj_bdef(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_obj *)((uint8_t *)blob + hdr->obj_def.start));
}

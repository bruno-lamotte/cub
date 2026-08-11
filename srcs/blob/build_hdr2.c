/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_hdr2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	fill_hdr_seg(t_blob_seg *s, size_t start, size_t size, uint8_t acc)
{
	if (!size_to_u32(start, &s->start))
		return (false);
	if (!size_to_u32(start + size, &s->end))
		return (false);
	if (!size_to_u32(size, &s->used))
		return (false);
	s->u_data.count = acc;
	return (true);
}

bool	fill_blob_hdr_seg_map(t_blob_seg *s, size_t start, size_t size,
			int *wh)
{
	if (!size_to_u32(start, &s->start))
		return (false);
	if (!size_to_u32(start + size, &s->end))
		return (false);
	if (!size_to_u32(size, &s->used))
		return (false);
	s->u_data.map_data.width = wh[0];
	s->u_data.map_data.height = wh[1];
	return (true);
}

bool	sub_build_hdr(t_data *d, t_blob_hdr *h, t_tmp_header *t, size_t *off)
{
	if (!fill_hdr_seg(&h->door_rt, *off, t->door_rt_size, d->door_rt_count))
		return (false);
	*off += t->door_rt_size;
	if (!fill_hdr_seg(&h->mstr_rt, *off, t->monster_rt_size,
			d->monster_rt_count))
		return (false);
	*off += t->monster_rt_size;
	if (!fill_hdr_seg(&h->wall_def, *off, t->wall_size, d->walls_len))
		return (false);
	*off += t->wall_size;
	if (!fill_hdr_seg(&h->air_def, *off, t->air_size, d->airs_len))
		return (false);
	*off += t->air_size;
	if (!fill_hdr_seg(&h->door_def, *off, t->door_size, d->doors_len))
		return (false);
	*off += t->door_size;
	return (true);
}

bool	sub_build_hdr2(t_data *d, t_blob_hdr *h, t_tmp_header *t, size_t *off)
{
	if (!fill_hdr_seg(&h->mstr_def, *off, t->monster_size, d->monsters_len))
		return (false);
	*off += t->monster_size;
	if (!fill_hdr_seg(&h->obj_def, *off, t->obj_size, d->obj_len))
		return (false);
	*off += t->obj_size;
	if (!fill_hdr_seg(&h->tex_def, *off, t->tex_size, d->textures_len))
		return (false);
	*off += t->tex_size;
	return (true);
}

bool	build_hdr(t_data *d, void *blob, t_tmp_header *tmp)
{
	t_blob_hdr	*h;
	size_t		off;

	h = (t_blob_hdr *)blob;
	off = tmp->header_size;
	if (!fill_blob_hdr_seg_map(&h->map, off, tmp->map_soa_size,
			(int [2]){(int)d->m_max_width, (int)d->m_height}))
		return (false);
	off += tmp->map_soa_size;
	if (!fill_hdr_seg(&h->pixels_data, off, tmp->px_size, 0))
		return (false);
	off += tmp->px_size;
	if (!fill_hdr_seg(&h->lut_def, off, tmp->lut_size, 1))
		return (false);
	off += tmp->lut_size;
	if (!sub_build_hdr(d, h, tmp, &off) || !sub_build_hdr2(d, h, tmp, &off))
		return (false);
	h->end_offset = off;
	return (true);
}

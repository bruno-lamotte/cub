#include "../../includes/cub.h"

bool inc_size_t_to_64_mult(size_t *val)
{
	if ((*val % 64) == 0)
		return (true);
	if (*val > SIZE_MAX - 64)
		return (false);
	while ((*val % 64) != 0)
		(*val)++;
	return (true);
}

bool	fill_tmp_hdr(t_data *d, t_tmp_header *hdr)
{
	size_t	tmp;
	size_t	one_px_size;
	int		i;
	// int		y;

	i = 0;
	// y = 0;
	hdr->px_size = 0;
	while (i < d->textures_len)
	{
		if (!d->textures_defs[i].is_color)
		{
			if (__builtin_mul_overflow((size_t)d->img_tab[i].width, (size_t)d->img_tab[i].height, &tmp) || \
				__builtin_mul_overflow(tmp, sizeof(uint32_t), &one_px_size) || \
				__builtin_add_overflow(hdr->px_size, one_px_size, &hdr->px_size) || \
				!inc_size_t_to_64_mult(&hdr->px_size))
				return (false);
			// y++;
		}
		i++;
	}
	hdr->header_size = sizeof(t_blob_hdr);
	if (__builtin_mul_overflow(sizeof(t_door_rt), (size_t)d->door_rt_count, &hdr->door_rt_size) || \
		!inc_size_t_to_64_mult(&hdr->door_rt_size) || \
		__builtin_mul_overflow(sizeof(t_monster_rt), (size_t)d->monster_rt_count, &hdr->monster_rt_size) || \
		!inc_size_t_to_64_mult(&hdr->monster_rt_size) || \
		__builtin_mul_overflow((size_t)d->m_height, (size_t)d->m_max_width, &tmp) || \
		!inc_size_t_to_64_mult(&tmp) || \
		__builtin_mul_overflow(tmp, MAP_SOA_LAYERS, &hdr->map_soa_size) || \
		__builtin_mul_overflow(sizeof(t_bdef_wall), (size_t)d->walls_len, &hdr->wall_size) || \
		!inc_size_t_to_64_mult(&hdr->wall_size) || \
		__builtin_mul_overflow(sizeof(t_bdef_air), (size_t)d->airs_len, &hdr->air_size) || \
		!inc_size_t_to_64_mult(&hdr->air_size) || \
		__builtin_mul_overflow(sizeof(t_bdef_door), (size_t)d->doors_len, &hdr->door_size) || \
		!inc_size_t_to_64_mult(&hdr->door_size) || \
		__builtin_mul_overflow(sizeof(t_bdef_monster), (size_t)d->monsters_len, &hdr->monster_size) || \
		!inc_size_t_to_64_mult(&hdr->monster_size) || \
		__builtin_mul_overflow(sizeof(t_bdef_obj), (size_t)d->obj_len, &hdr->obj_size) || \
		!inc_size_t_to_64_mult(&hdr->obj_size) || \
		__builtin_mul_overflow(sizeof(t_bdef_tex), (size_t)d->textures_len, &hdr->tex_size) || \
		!inc_size_t_to_64_mult(&hdr->tex_size))
		return (false);
	hdr->lut_size = sizeof(t_lut);
	if (!inc_size_t_to_64_mult(&hdr->lut_size))
		return (false);
	return (true);
}

bool get_alloc_size(t_tmp_header *hdr, size_t *out)
{
	*out = 0;
	if (__builtin_add_overflow(0, hdr->header_size, out) || \
		__builtin_add_overflow(hdr->door_rt_size, *out, out) || \
		__builtin_add_overflow(hdr->monster_rt_size, *out, out) || \
		__builtin_add_overflow(hdr->map_soa_size, *out, out) || \
		__builtin_add_overflow(hdr->wall_size, *out, out) || \
		__builtin_add_overflow(hdr->air_size, *out, out) || \
		__builtin_add_overflow(hdr->door_size, *out, out) || \
		__builtin_add_overflow(hdr->monster_size, *out, out) || \
		__builtin_add_overflow(hdr->obj_size, *out, out) || \
		__builtin_add_overflow(hdr->tex_size, *out, out) || \
		__builtin_add_overflow(hdr->px_size, *out, out) || \
		__builtin_add_overflow(hdr->lut_size, *out, out))
		return (false);
	return (true);
}

bool size_to_u32(size_t val, uint32_t *out)
{
    return !__builtin_add_overflow(val, 0, out);
}

bool fill_blob_hdr_seg(t_blob_seg *seg, size_t start, size_t size, uint8_t count)
{
	if (!size_to_u32(start, &seg->start))
		return (false);
	if (!size_to_u32(start + size, &seg->end))
		return (false);
	if (!size_to_u32(size, &seg->used))
		return (false);
	seg->count = count;
	return (true);
}

bool fill_blob_hdr_seg_map(t_blob_seg *seg, size_t start, size_t size, uint16_t width, uint16_t height)
{
	if (!size_to_u32(start, &seg->start))
		return (false);
	if (!size_to_u32(start + size, &seg->end))
		return (false);
	if (!size_to_u32(size, &seg->used))
		return (false);
	seg->map_data.width = width;
	seg->map_data.height = height;
	return (true);
}

bool build_hdr(t_data *d, void *blob, t_tmp_header *tmp)
{
	t_blob_hdr *h;
	size_t off;

	h = (t_blob_hdr *)blob;
	off = tmp->header_size;
	if (!fill_blob_hdr_seg_map(&h->map, off, tmp->map_soa_size, d->m_max_width, d->m_height))
		return (false);
	off += tmp->map_soa_size;
	if (!fill_blob_hdr_seg(&h->door_rt, off, tmp->door_rt_size, d->door_rt_count))
		return (false);
	off += tmp->door_rt_size;
	if (!fill_blob_hdr_seg(&h->mstr_rt, off, tmp->monster_rt_size, d->monster_rt_count))
		return (false);
	off += tmp->monster_rt_size;
	if (!fill_blob_hdr_seg(&h->wall_def, off, tmp->wall_size, d->walls_len))
		return (false);
	off += tmp->wall_size;
	if (!fill_blob_hdr_seg(&h->air_def, off, tmp->air_size, d->airs_len))
		return (false);
	off += tmp->air_size;
	if (!fill_blob_hdr_seg(&h->door_def, off, tmp->door_size, d->doors_len))
		return (false);
	off += tmp->door_size;
	if (!fill_blob_hdr_seg(&h->mstr_def, off, tmp->monster_size, d->monsters_len))
		return (false);
	off += tmp->monster_size;
	if (!fill_blob_hdr_seg(&h->obj_def, off, tmp->obj_size, d->obj_len))
		return (false);
	off += tmp->obj_size;
	if (!fill_blob_hdr_seg(&h->tex_def, off, tmp->tex_size, d->textures_len))
		return (false);
	off += tmp->tex_size;
	if (!fill_blob_hdr_seg(&h->pixels_data, off, tmp->px_size, 0))
		return (false);
	off += tmp->px_size;
	if (!fill_blob_hdr_seg(&h->lut_def, off, tmp->lut_size, 1))
        return (false);
    off += tmp->lut_size;
	h->end_offset = off;
	return (true);
}

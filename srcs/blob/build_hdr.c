/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_hdr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	inc_size_t_to_64_mult(size_t *val)
{
	if ((*val % 64) == 0)
		return (true);
	if (*val > SIZE_MAX - 64)
		return (false);
	while ((*val % 64) != 0)
		(*val)++;
	return (true);
}

static bool	sub_fill_defs_hdr(t_data *d, t_tmp_header *hdr)
{
	if (__builtin_mul_overflow(sizeof(t_bdef_wall),
			(size_t)d->walls_len, &hdr->wall_size)
		|| !inc_size_t_to_64_mult(&hdr->wall_size)
		|| __builtin_mul_overflow(sizeof(t_bdef_air),
			(size_t)d->airs_len, &hdr->air_size)
		|| !inc_size_t_to_64_mult(&hdr->air_size)
		|| __builtin_mul_overflow(sizeof(t_bdef_door),
			(size_t)d->doors_len, &hdr->door_size)
		|| !inc_size_t_to_64_mult(&hdr->door_size)
		|| __builtin_mul_overflow(sizeof(t_bdef_monster),
			(size_t)d->monsters_len, &hdr->monster_size)
		|| !inc_size_t_to_64_mult(&hdr->monster_size)
		|| __builtin_mul_overflow(sizeof(t_bdef_obj),
			(size_t)d->obj_len, &hdr->obj_size)
		|| !inc_size_t_to_64_mult(&hdr->obj_size)
		|| __builtin_mul_overflow(sizeof(t_bdef_tex),
			(size_t)d->textures_len, &hdr->tex_size)
		|| !inc_size_t_to_64_mult(&hdr->tex_size))
		return (false);
	return (true);
}

bool	sub_fill_map_hdr(t_data *d, t_tmp_header *hdr, size_t *tmp)
{
	if (__builtin_mul_overflow(sizeof(t_door_rt),
			(size_t)d->door_rt_count, &hdr->door_rt_size)
		|| !inc_size_t_to_64_mult(&hdr->door_rt_size)
		|| __builtin_mul_overflow(sizeof(t_monster_rt),
			(size_t)d->monster_rt_count, &hdr->monster_rt_size)
		|| !inc_size_t_to_64_mult(&hdr->monster_rt_size)
		|| __builtin_mul_overflow((size_t)d->m_height,
			(size_t)d->m_max_width, tmp) || !inc_size_t_to_64_mult(tmp)
		|| __builtin_mul_overflow(*tmp, MAP_SOA_LAYERS, &hdr->map_soa_size))
		return (false);
	return (sub_fill_defs_hdr(d, hdr));
}

bool	fill_tmp_hdr(t_data *d, t_tmp_header *hdr)
{
	size_t	tmp;
	size_t	one_px_size;
	int		i;

	i = 0;
	hdr->px_size = 0;
	while (i < d->textures_len)
	{
		if (!d->textures_defs[i].is_color)
		{
			if (__builtin_mul_overflow((size_t)d->img_tab[i].width,
					(size_t)d->img_tab[i].height, &tmp)
				|| __builtin_mul_overflow(tmp, sizeof(uint32_t), &one_px_size)
				|| __builtin_add_overflow(hdr->px_size, one_px_size,
					&hdr->px_size) || !inc_size_t_to_64_mult(&hdr->px_size))
				return (false);
		}
		i++;
	}
	hdr->header_size = sizeof(t_blob_hdr);
	if (!sub_fill_map_hdr(d, hdr, &tmp))
		return (false);
	hdr->lut_size = sizeof(t_lut);
	return (inc_size_t_to_64_mult(&hdr->lut_size));
}

bool	get_alloc_size(t_tmp_header *hdr, size_t *out)
{
	*out = 0;
	if (__builtin_add_overflow(0, hdr->header_size, out)
		|| __builtin_add_overflow(hdr->door_rt_size, *out, out)
		|| __builtin_add_overflow(hdr->monster_rt_size, *out, out)
		|| __builtin_add_overflow(hdr->map_soa_size, *out, out)
		|| __builtin_add_overflow(hdr->wall_size, *out, out)
		|| __builtin_add_overflow(hdr->air_size, *out, out)
		|| __builtin_add_overflow(hdr->door_size, *out, out)
		|| __builtin_add_overflow(hdr->monster_size, *out, out)
		|| __builtin_add_overflow(hdr->obj_size, *out, out)
		|| __builtin_add_overflow(hdr->tex_size, *out, out)
		|| __builtin_add_overflow(hdr->px_size, *out, out)
		|| __builtin_add_overflow(hdr->lut_size, *out, out))
		return (false);
	return (true);
}

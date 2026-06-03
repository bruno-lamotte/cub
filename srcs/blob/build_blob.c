/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_blob.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 19:40:31 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/02 20:08:19 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	sub_build_px(t_data *d, void *blob, char *src_pixel, ssize_t i)
{
	t_tmp_build_px	tmp;
	ssize_t			y;
	ssize_t			x;

	tmp.tex_defs = get_tex_bdef(blob);
	tmp.dest = (uint32_t *)((uint8_t *)blob + \
((t_blob_hdr *)blob)->pixels_data.start);
	if (!d->textures_defs[i].is_color)
	{
		tmp.src_img = &d->img_tab[i];
		tmp.tex_dest = (uint32_t *)((uint8_t *)tmp.dest + \
tmp.tex_defs[i].offset);
		y = -1;
		while (++y < tmp.src_img->height)
		{
			x = -1;
			while (++x < tmp.src_img->width)
			{
				src_pixel = tmp.src_img->addr + \
(y * tmp.src_img->line_len + x * (tmp.src_img->bpp / 8));
				tmp.tex_dest[y * tmp.src_img->width + x] = \
*(uint32_t *)src_pixel;
			}
		}
	}
}

void	build_px(t_data *d, void *blob)
{
	ssize_t		i;

	i = -1;
	while (++i < d->textures_len)
		sub_build_px(d, blob, NULL, i);
}

bool	build(t_data *data, void *blob, t_tmp_header *tmp_header)
{
	if (!build_hdr(data, blob, tmp_header))
		return (printf("ERR 1\n"), false);
	build_rt(data, blob);
	build_bdef(data, blob);
	build_px(data, blob);
	return (true);
}

bool	build_blob(t_data *d, void **blob)
{
	size_t			alloc_size;
	t_tmp_header	tmp_header;

	if (!fill_tmp_hdr(d, &tmp_header) || \
!get_alloc_size(&tmp_header, &alloc_size))
		return (false);
	*blob = ft_calloc(1, alloc_size);
	if (!*blob)
		return (false);
	if (!build(d, *blob, &tmp_header))
		return (free(*blob), false);
	return (true);
}

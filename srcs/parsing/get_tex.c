/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:32:00 by rstarcev          #+#    #+#             */
/*   Updated: 2026/05/28 14:43:51 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	free_img_tab(void *mlx, t_img *img, size_t lim)
{
	size_t	i;

	i = 0;
	while (i < lim)
	{
		if (!img[i].is_color && img[i].img_ptr)
			mlx_destroy_image(mlx, img[i].img_ptr);
		i++;
	}
	free(img);
}

bool	get_one_xpm(void *mlx, char *path, t_img *img)
{
	if (!mlx || !path || !img)
		return (false);
	img->img_ptr = mlx_xpm_file_to_image(mlx, path, &img->width, &img->height);
	if (!img->img_ptr)
		return (false);
	img->addr = \
mlx_get_data_addr(img->img_ptr, &img->bpp, &img->line_len, &img->endian);
	if (!img->addr || img->bpp != 32)
	{
		mlx_destroy_image(mlx, img->img_ptr);
		img->img_ptr = NULL;
		return (false);
	}
	return (true);
}

bool	get_all_tex(t_data *data, t_screen *s)
{
	ssize_t			i;
	size_t			len;
	t_img			*g;
	t_texture_def	*defs;
	t_index			idx;

	ft_memset(&idx, 0, sizeof(t_index));
	defs = data->textures_defs;
	len = data->textures_len;
	g = ft_calloc(len, sizeof(t_img));
	if (!g)
		return (idx.err = PERR_MALLOC_ERR, print_perr(&idx), 0);
	i = -1;
	while (++i < len)
	{
		if (defs[i].is_color)
			g[i].hex_color = defs[i].color;
		if (defs[i].is_color)
			g[i].is_color = true;
		else if (!get_one_xpm(s->mlx_ptr, defs[i].path, &g[i]))
			return (idx.err = PERR_MLX_ERROR, free_img_tab(s->mlx_ptr, g, i), \
print_perr(&idx), false);
		if (!defs[i].is_color)
			mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, g[i].img_ptr, 0, 0);
	}
	return (data->img_tab = g, true);
}

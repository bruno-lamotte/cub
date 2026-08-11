/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_background.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:30:09 by rstarcev          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	get_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

uint32_t	get_menu_pixel_img(t_img *img, int32_t x, int32_t y)
{
	char	*src;

	if (!img || !img->addr)
		return (0);
	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return (0);
	src = img->addr + (y * img->line_len) + (x * (img->bpp / 8));
	return (*(uint32_t *)src);
}

void	put_pixel_img(t_img *img, int x, int y, uint32_t color)
{
	char	*dst;

	if (!img || !img->addr)
		return ;
	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return ;
	dst = img->addr + (y * img->line_len) + (x * (img->bpp / 8));
	*(uint32_t *)dst = color;
}

bool	load_xpm_img(void *mlx, t_img *img, char *path)
{
	img->img_ptr = mlx_xpm_file_to_image(mlx, path, &img->width, &img->height);
	if (!img->img_ptr)
		return (false);
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bpp, \
&img->line_len, &img->endian);
	if (!img->addr)
		return (false);
	return (true);
}

bool	create_scaled_bg(t_menu *m)
{
	m->bg_scaled.width = m->w_width;
	m->bg_scaled.height = m->w_height;
	m->bg_scaled.img_ptr = mlx_new_image(m->mlx_ptr, m->bg_scaled.width, \
m->bg_scaled.height);
	if (!m->bg_scaled.img_ptr)
		return (menu_error("mlx_new_image failure"), false);
	m->bg_scaled.addr = mlx_get_data_addr(m->bg_scaled.img_ptr, \
&m->bg_scaled.bpp, &m->bg_scaled.line_len, &m->bg_scaled.endian);
	if (!m->bg_scaled.addr)
		return (menu_error("mlx_get_data_addr failed"), false);
	return (true);
}

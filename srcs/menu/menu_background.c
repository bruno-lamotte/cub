/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_background.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:30:09 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/05 18:44:46 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

static int	get_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

static uint32_t	get_pixel_img(t_img *img, int32_t x, int32_t y)
{
	char	*src;

	if (!img || !img->addr)
		return (0);
	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return (0);
	src = img->addr + (y * img->line_len) + (x * (img->bpp / 8));
	return (*(uint32_t *)src);
}

static void	put_pixel_img(t_img *img, int x, int y, uint32_t color)
{
	char	*dst;

	if (!img || !img->addr)
		return ;
	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return ;
	dst = img->addr + (y * img->line_len) + (x * (img->bpp / 8));
	*(uint32_t *)dst = color;
}

static bool	load_xpm_img(void *mlx, t_img *img, char *path)
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

static bool	create_scaled_bg(t_menu *m)
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

static t_bg_data	build_bg_data(t_menu *m)
{
	t_bg_data	data;

	data.scale = get_max((m->w_width * 1000) / m->bg_src.width, \
(m->w_height * 1000) / m->bg_src.height);
	data.scaled_w = (m->bg_src.width * data.scale) / 1000;
	data.scaled_h = (m->bg_src.height * data.scale) / 1000;
	data.off_x = (data.scaled_w - m->w_width) / 2;
	data.off_y = (data.scaled_h - m->w_height) / 2;
	return (data);
}

static void	put_scaled_bg_px(t_menu *m, t_bg_data *d, int32_t x, int32_t y)
{
	int32_t		src_x;
	int32_t		src_y;
	uint32_t	color;

	src_x = ((x + d->off_x) * m->bg_src.width) / d->scaled_w;
	src_y = ((y + d->off_y) * m->bg_src.height) / d->scaled_h;
	color = get_pixel_img(&m->bg_src, src_x, src_y);
	put_pixel_img(&m->bg_scaled, x, y, color);
}

static void	fill_scaled_bg(t_menu *m)
{
	int32_t		x;
	int32_t		y;
	t_bg_data	data;

	data = build_bg_data(m);
	y = 0;
	while (y < m->w_height)
	{
		x = 0;
		while (x < m->w_width)
		{
			put_scaled_bg_px(m, &data, x, y);
			x++;
		}
		y++;
	}
}

bool	init_menu_bg(t_menu *m)
{
	if (!load_xpm_img(m->mlx_ptr, &m->bg_src, MENU_BG_PATH))
		return (false);
	if (m->bg_src.width <= 0 || m->bg_src.height <= 0)
		return (false);
	if (!create_scaled_bg(m))
		return (false);
	fill_scaled_bg(m);
	return (true);
}

void	destroy_menu_bg(t_menu *m)
{
	if (m->bg_scaled.img_ptr)
	{
		mlx_destroy_image(m->mlx_ptr, m->bg_scaled.img_ptr);
		m->bg_scaled.img_ptr = NULL;
	}
	if (m->bg_src.img_ptr)
	{
		mlx_destroy_image(m->mlx_ptr, m->bg_src.img_ptr);
		m->bg_src.img_ptr = NULL;
	}
}

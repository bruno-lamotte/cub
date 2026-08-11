/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_background2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int			get_max(int a, int b);
uint32_t	get_menu_pixel_img(t_img *img, int32_t x, int32_t y);
void		put_pixel_img(t_img *img, int x, int y, uint32_t color);
bool		load_xpm_img(void *mlx, t_img *img, char *path);
bool		create_scaled_bg(t_menu *m);

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
	color = get_menu_pixel_img(&m->bg_src, src_x, src_y);
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

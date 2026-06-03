/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:45:02 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/03 00:22:37 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	free_screen(t_screen *s)
{
	if (!s)
		return ;
	if (s->img.img_ptr)
		mlx_destroy_image(s->mlx_ptr, s->img.img_ptr);
	if (s->img2.img_ptr)
		mlx_destroy_image(s->mlx_ptr, s->img2.img_ptr);
	if (s->win_ptr)
		mlx_destroy_window(s->mlx_ptr, s->win_ptr);
	if (s->mlx_ptr)
	{
		mlx_destroy_display(s->mlx_ptr);
		free(s->mlx_ptr);
	}
}

static bool	sub_init_mlx(t_screen *screen)
{
	int	screen_w;
	int	screen_h;

	screen->mlx_ptr = mlx_init();
	if (!screen->mlx_ptr)
		return (false);
	mlx_get_screen_size(screen->mlx_ptr, &screen_w, &screen_h);
	screen->win_width = (screen_w * 9) / 10;
	screen->win_height = (screen_h * 9) / 10;
	screen->img.width = screen->win_width;
	screen->img.height = screen->win_height;
	screen->img2.width = screen->win_width;
	screen->img2.height = screen->win_height;
	return (true);
}

bool	init_mlx(t_screen *screen, char *title)
{
	if (!sub_init_mlx(screen))
		return (false);
	screen->win_ptr = mlx_new_window(screen->mlx_ptr, \
screen->win_width, screen->win_height, title);
	if (!screen->win_ptr)
		return (free_screen(screen), false);
	screen->img.img_ptr = mlx_new_image(screen->mlx_ptr, \
screen->win_width, screen->win_height);
	if (!screen->img.img_ptr)
		return (free_screen(screen), false);
	screen->img.addr = mlx_get_data_addr(screen->img.img_ptr, \
&screen->img.bpp, &screen->img.line_len, &screen->img.endian);
	if (!screen->img.addr)
		return (free_screen(screen), false);
	screen->img2.img_ptr = mlx_new_image(screen->mlx_ptr, \
screen->win_width, screen->win_height);
	if (!screen->img2.img_ptr)
		return (free_screen(screen), false);
	screen->img2.addr = mlx_get_data_addr(screen->img2.img_ptr, \
&screen->img2.bpp, &screen->img2.line_len, &screen->img2.endian);
	if (!screen->img2.addr)
		return (free_screen(screen), false);
	return (true);
}

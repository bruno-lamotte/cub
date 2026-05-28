/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:45:02 by rstarcev          #+#    #+#             */
/*   Updated: 2026/05/28 14:50:44 by rstarcev         ###   ########.fr       */
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

int g_win_width = 1200;
int g_win_height = 800;

bool	init_mlx(t_screen *screen, char *title)
{
	int	screen_w;
	int	screen_h;

	screen->mlx_ptr = mlx_init();
	if (!screen->mlx_ptr)
		return (false);
	mlx_get_screen_size(screen->mlx_ptr, &screen_w, &screen_h);
	g_win_width = (screen_w * 9) / 10;
	g_win_height = (screen_h * 9) / 10;
	screen->win_ptr = mlx_new_window(screen->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, title);
	if (!screen->win_ptr)
		return (free_screen(screen), false);
	screen->img.img_ptr = mlx_new_image(screen->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!screen->img.img_ptr)
		return (free_screen(screen), false);
	screen->img.addr = mlx_get_data_addr(screen->img.img_ptr, &screen->img.bpp, &screen->img.line_len, &screen->img.endian);
	if (!screen->img.addr)
		return (free_screen(screen), false);
	screen->img2.img_ptr = mlx_new_image(screen->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!screen->img2.img_ptr)
		return (free_screen(screen), false);
	screen->img2.addr = mlx_get_data_addr(screen->img2.img_ptr, &screen->img2.bpp, &screen->img2.line_len, &screen->img2.endian);
	if (!screen->img2.addr)
		return (free_screen(screen), false);
	return (true);
}

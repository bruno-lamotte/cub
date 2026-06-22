/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:29:50 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/05 18:30:43 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

static char	*get_map_name(char *path)
{
	char	*last_occ;

	last_occ = ft_strrchr(path, '/');
	if (!last_occ)
		return (path);
	return (last_occ + 1);
}

static void	render_map_name(t_menu *m, int32_t i, int32_t y)
{
	int32_t	x;
	char	*name;

	x = m->w_width / 2;
	name = get_map_name(m->maps[i]);
	if (i == m->selected)
	{
		mlx_string_put(m->mlx_ptr, m->win_ptr, x - 30, y, X_GREEN, ">");
		mlx_string_put(m->mlx_ptr, m->win_ptr, x, y, X_GREEN, name);
	}
	else
		mlx_string_put(m->mlx_ptr, m->win_ptr, x, y, X_WHITE, name);
}

void	render_menu(t_menu *m)
{
	int32_t	i;
	int32_t	max;
	int32_t	y;

	i = m->selected;
	if (i >= 5)
	{
		if (i >= m->map_count - 5)
			i -= 5;
		else
			i -= 5;
	}
	else
		i = 0;
	max = i + 11;
	y = m->w_height / 3;
	mlx_put_image_to_window(m->mlx_ptr, m->win_ptr, m->bg_scaled.img_ptr, 0, 0);
	while (i < m->map_count && i < max)
	{
		render_map_name(m, i, y);
		y += LINE_HEIGHT;
		i++;
	}
}

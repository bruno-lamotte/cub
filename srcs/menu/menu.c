/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:28:30 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/05 18:29:39 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	menu_error(char *msg)
{
	write(2, "menu error: ", 12);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

static bool	init_menu(t_menu *m)
{
	int32_t	s_width;
	int32_t	s_height;

	ft_bzero(m, sizeof(t_menu));
	m->mlx_ptr = mlx_init();
	if (!m->mlx_ptr)
		return (false);
	mlx_get_screen_size(m->mlx_ptr, &s_width, &s_height);
	m->w_width = (s_width * 9) / 10;
	m->w_height = (s_height * 9) / 10;
	m->win_ptr = mlx_new_window(m->mlx_ptr, m->w_width, m->w_height, \
"CUB 3D: Menu");
	if (!m->win_ptr)
		return (false);
	if (!init_menu_bg(m))
		return (false);
	m->maps = load_maps(MAP_FOLDER, &m->map_count);
	if (!m->maps)
		m->map_count = 0;
	return (true);
}

char	*menu(void)
{
	t_menu	menu;
	char	*out;

	out = NULL;

	if (!init_menu(&menu))
		return (NULL);
	render_menu(&menu);
	mlx_hook(menu.win_ptr, KeyPress, KeyPressMask, \
(void *)menu_key_hook, &menu);
	mlx_loop(menu.mlx_ptr);
	if (menu.ret_val == MENU_PLAY)
	{
		out = ft_strdup(menu.maps[menu.selected]);
		if (!out)
			menu.ret_val = MENU_ERROR;
	}
	free_str_tab(menu.maps);
	destroy_menu_window(&menu);
	if (menu.ret_val == MENU_PLAY)
		return (out);
	return (NULL);
}

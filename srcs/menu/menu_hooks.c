/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:30:03 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/05 18:41:50 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	destroy_menu_window(t_menu *menu)
{
	destroy_menu_bg(menu);
	if (menu->win_ptr)
	{
		mlx_destroy_window(menu->mlx_ptr, menu->win_ptr);
		menu->win_ptr = NULL;
	}
	if (menu->mlx_ptr)
	{
		mlx_destroy_display(menu->mlx_ptr);
		free(menu->mlx_ptr);
		menu->mlx_ptr = NULL;
	}
}

void	close_menu(t_menu *menu)
{
	free_str_tab(menu->maps);
	destroy_menu_window(menu);
	exit(0);
}

bool	launch_selected_map(t_menu *menu)
{
	char	*selected_map;

	selected_map = ft_strdup(menu->maps[menu->selected]);
	if (!selected_map)
		return (false);
	free_str_tab(menu->maps);
	destroy_menu_window(menu);
	cub(selected_map);
	free(selected_map);
	return (true);
}

int	menu_key_hook(int keycode, t_menu *menu)
{
	if (keycode == XK_Escape)
		close_menu(menu);
	else if (keycode == XK_Up && menu->map_count > 0)
	{
		menu->selected--;
		if (menu->selected < 0)
			menu->selected = menu->map_count - 1;
		render_menu(menu);
	}
	else if (keycode == XK_Down && menu->map_count > 0)
	{
		menu->selected++;
		if (menu->selected >= menu->map_count)
			menu->selected = 0;
		render_menu(menu);
	}
	else if ((keycode == XK_Return || keycode == XK_KP_Enter) \
&& menu->map_count > 0)
	{
		menu->ret_val = MENU_PLAY;
		mlx_loop_end(menu->mlx_ptr);
	}
	return (0);
}

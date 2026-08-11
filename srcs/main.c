/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	cub(char *map)
{
	t_Aengine	a;
	t_engine	e;

	if (!build_engine(map, &a, &e))
		return (CUB_EXIT_ERROR);
	e.ret_val = CUB_EXIT_QUIT;
	mlx_hook(e.screen->win_ptr, 2, 1L << 0, (void *)key_press, &e);
	mlx_hook(e.screen->win_ptr, 3, 1L << 1, (void *)key_release, &e);
	mlx_hook(e.screen->win_ptr, 6, 1L << 6, (void *)mouse_move, &e);
	mlx_hook(e.screen->win_ptr, 17, 0L, (void *)close_window, &e);
	mlx_loop_hook(e.screen->mlx_ptr, (void *)game_loop, &e);
	mlx_loop(e.screen->mlx_ptr);
	shutdown_thread_pool(&e);
	if (e.slr)
		free_all(e.slr, e.slr->table);
	free(e.z_buffer);
	free(e.blob);
	free_engine_graphics(&e);
	return (e.ret_val);
}

int	cub_menu(void)
{
	char	*path;

	printf("Launching level selection menu...\n");
	while (1)
	{
		path = menu();
		if (!path)
			return (1);
		if (cub(path) == CUB_EXIT_ERROR)
		{
			free(path);
			return (1);
		}
		free(path);
	}
	return (0);
}

int	main(int ac, char **av)
{
	if (ac == 1)
		return (cub_menu());
	return (cub(av[1]));
}

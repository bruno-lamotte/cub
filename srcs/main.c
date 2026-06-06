/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 00:12:30 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/06 21:59:32 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

bool	build_abstract_engine(t_Aengine *e, char *map_name)
{
	ft_bzero(e, sizeof(t_Aengine));
	if (!parse_file(map_name, &e->data))
		return (false);
	if (!init_mlx(&e->screen, "SUPER FENETRE"))
		return (free_preprocessing_data(&e->data), false);
	if (!get_all_tex(&e->data, &e->screen))
		return (free_screen(&e->screen), free_preprocessing_data(&e->data), 0);
	if (!build_blob(&e->data, &e->blob))
		return (free_img_tab(e->screen.mlx_ptr, e->data.img_tab, \
e->data.textures_len), free_screen(&e->screen), \
free_preprocessing_data(&e->data), false);
	init_player_rt(&e->data, &e->player);
	return (true);
}

bool	build_engine(char *map_name, t_Aengine *a, t_engine *e)
{
	memset(e, 0, sizeof(t_engine));
	if (!build_abstract_engine(a, map_name))
		return (printf(RED"ERRROR 3\n"RST), false);
	e->data = &a->data;
	e->player = &a->player;
	e->blob = a->blob;
	e->screen = &a->screen;
	e->slr = &a->slr;
	e->z_buffer = malloc(sizeof(t_ray_data) * e->screen->win_width);
	if (!e->z_buffer)
		return (free_img_tab(e->screen->mlx_ptr, e->data->img_tab, \
e->data->textures_len), free_screen(e->screen), \
free_preprocessing_data(e->data), false);
	init_thread_pool(e);
	init_static_lights(e);
	load_monster_anim(e);
	initialize_data(e->slr);
	e->slr->table = create_parsing_table(e->slr);
	if (!e->slr->table)
		printf("Failed to generate SLR(1) parsing table\n");
	else
		init_all_ai_behaviors(e->slr, e);
	return (true);
}

int	cub(char *map)
{
	t_Aengine	a;
	t_engine	e;

	if (!build_engine(map, &a, &e))
		return (CUB_EXIT_ERROR);
	e.ret_val = CUB_EXIT_QUIT;
	print_parsing(e.data);
	print_blob(e.blob);
	mlx_hook(e.screen->win_ptr, 2, 1L << 0, (void *)key_press, &e);
	mlx_hook(e.screen->win_ptr, 3, 1L << 1, (void *)key_release, &e);
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
	char *path;

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
	else
		return (cub(av[1]));
}

// int main(void)
// {
// 	return (cub("./maps/1.cub"));
// }

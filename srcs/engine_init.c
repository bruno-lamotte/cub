/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

static bool	sub_abstract(t_Aengine *e)
{
	if (!get_all_tex(&e->data, &e->screen))
	{
		free_screen(&e->screen);
		free_preprocessing_data(&e->data);
		return (false);
	}
	if (!build_blob(&e->data, &e->blob))
	{
		free_img_tab(e->screen.mlx_ptr, e->data.img_tab, e->data.textures_len);
		free_screen(&e->screen);
		free_preprocessing_data(&e->data);
		return (false);
	}
	return (true);
}

bool	build_abstract_engine(t_Aengine *e, char *map_name)
{
	ft_bzero(e, sizeof(t_Aengine));
	if (!parse_file(map_name, &e->data))
		return (false);
	if (!init_mlx(&e->screen, "SUPER FENETRE"))
	{
		free_preprocessing_data(&e->data);
		return (false);
	}
	if (!sub_abstract(e))
		return (false);
	init_player_rt(&e->data, &e->player);
	return (true);
}

static bool	load_engine_assets(t_engine *e)
{
	if (!get_one_xpm(e->screen->mlx_ptr, "assets/lampe.xpm", &e->lamp_tex))
	{
		printf("Error: Failed to load assets/lampe.xpm\n");
		free_img_tab(e->screen->mlx_ptr, e->data->img_tab,
			e->data->textures_len);
		free_screen(e->screen);
		free_preprocessing_data(e->data);
		return (false);
	}
	if (!get_one_xpm(e->screen->mlx_ptr, "assets/terminal.xpm",
			&e->terminal_tex))
	{
		printf("Error: Failed to load assets/terminal.xpm\n");
		free_img_tab(e->screen->mlx_ptr, e->data->img_tab,
			e->data->textures_len);
		free_screen(e->screen);
		free_preprocessing_data(e->data);
		return (false);
	}
	return (true);
}

static bool	sub_build_engine(t_engine *e)
{
	if (!load_engine_assets(e))
		return (false);
	initialize_data(e->slr);
	e->slr->table = create_parsing_table(e->slr);
	if (e->slr->table)
		init_all_ai_behaviors(e->slr, e);
	return (true);
}

bool	build_engine(char *map_name, t_Aengine *a, t_engine *e)
{
	ft_bzero(e, sizeof(t_engine));
	if (!build_abstract_engine(a, map_name))
		return (printf(RED"ERRROR 3\n"RST), false);
	e->data = &a->data;
	e->player = &a->player;
	e->blob = a->blob;
	e->screen = &a->screen;
	e->slr = &a->slr;
	e->z_buffer = malloc(sizeof(t_ray_data) * e->screen->win_width);
	if (!e->z_buffer)
	{
		free_img_tab(e->screen->mlx_ptr, e->data->img_tab,
			e->data->textures_len);
		free_screen(e->screen);
		free_preprocessing_data(e->data);
		return (false);
	}
	init_thread_pool(e);
	init_static_lights(e);
	load_monster_anim(e);
	return (sub_build_engine(e));
}

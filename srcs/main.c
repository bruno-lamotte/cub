#include "../includes/cub.h"



int main(int ac, char **av)
{
	t_data		*data;
	t_screen	*screen;

	if (ac != 2)
		return (1);
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (printf("2\n"), 1);
	if (!parse_file(av[1], data))
		return (1);
	print_parsing(data);
	screen = ft_calloc(1, sizeof(t_screen));
	if (!screen)
		return (printf("2\n"), free_preprocessing_data(data), 1);
	if (!init_mlx(screen, "SUPER FENETRE"))
		return (free_preprocessing_data(data), 1);
	if (!get_all_tex(data, screen))
		return (printf("ERROR MLX\n"), free_screen(screen), free_preprocessing_data(data), 1);
	printf("BLOBBING\n");
	void *blob;
	if (!build_blob(data, &blob))
		return (free_img_tab(screen->mlx_ptr, data->img_tab, data->textures_len), free_screen(screen), free_preprocessing_data(data), free(data), free(screen), 1);

	t_engine *engine;
	engine = ft_calloc(1, sizeof(t_engine));
	if (!engine)
	{
		free(blob);
		return (free_img_tab(screen->mlx_ptr, data->img_tab, data->textures_len), free_screen(screen), free_preprocessing_data(data), free(data), free(screen), 1);
	}
	engine->screen = screen;
	engine->blob = blob;
	engine->data = data;
	engine->player = init_player_rt(data);
	if (!engine->player)
	{
		free(engine);
		free(blob);
		return (free_img_tab(screen->mlx_ptr, data->img_tab, data->textures_len), free_screen(screen), free_preprocessing_data(data), free(data), free(screen), 1);
	}
	engine->z_buffer = malloc(sizeof(t_ray_data) * screen->win_width);
	if (!engine->z_buffer)
	{
		free(engine->player);
		free(engine);
		free(blob);
		return (free_img_tab(screen->mlx_ptr, data->img_tab, data->textures_len), free_screen(screen), free_preprocessing_data(data), free(data), free(screen), 1);
	}
	init_thread_pool(engine);
	init_static_lights(engine);
	load_monster_anim(engine);
	if (!get_one_xpm(screen->mlx_ptr, "assets/lampe.xpm", &engine->lamp_tex))
	{
		printf("Error: Failed to load assets/lampe.xpm\n");
		close_window(engine);
	}
	if (!get_one_xpm(screen->mlx_ptr, "assets/terminal.xpm", &engine->terminal_tex))
	{
		printf("Error: Failed to load assets/terminal.xpm\n");
		close_window(engine);
	}

	engine->slr = malloc(sizeof(t_slr1));
	if (!engine->slr)
	{
		printf("Error: Failed to allocate memory for SLR parser\n");
		close_window(engine);
	}
	initialize_data(engine->slr);
	engine->slr->table = create_parsing_table(engine->slr);
	if (!engine->slr->table)
	{
		printf("Error: Failed to generate SLR(1) parsing table\n");
		close_window(engine);
	}
	init_all_ai_behaviors(engine->slr, engine);

	mlx_hook(screen->win_ptr, 2, 1L<<0, (void *)key_press, engine);
	mlx_hook(screen->win_ptr, 3, 1L<<1, (void *)key_release, engine);
	mlx_hook(screen->win_ptr, 17, 0L, (void *)close_window, engine);
	mlx_loop_hook(screen->mlx_ptr, (void *)game_loop, engine);

	// run game
	mlx_loop(screen->mlx_ptr);

	return (0);
}
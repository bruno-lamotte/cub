#include "../includes/cub.h"

int main(int ac, char **av)
{
	if (ac != 2)
		return (1);

	// init_data
	t_data *data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (printf("2\n"), 1);
	if (!parse_file(av[1], data))
		return (1);

	print_parsing(data);

	// init mlx
	t_screen *screen;

	screen = ft_calloc(1, sizeof(t_screen));
	if (!screen)
		return (free_preprocessing_data(data), 1);
	if (!init_mlx(screen, "SUPER FENETRE"))
		return (free_preprocessing_data(data), 1);
	if (!get_all_tex(data, screen))
		return (free_screen(screen), free_preprocessing_data(data), 1);
	// draw_tex_lib(screen, data);
	
	// build blob
	void *blob;
	if (!build_blob(data, &blob))
		return (free_img_tab(screen->mlx_ptr, data->img_tab, data->textures_len), free_screen(screen), free_preprocessing_data(data), free(data), free(screen), 1);

	// init engine
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
	engine->z_buffer = malloc(sizeof(t_ray_data) * WINDOW_WIDTH);
	if (!engine->z_buffer)
	{
		free(engine->player);
		free(engine);
		free(blob);
		return (free_img_tab(screen->mlx_ptr, data->img_tab, data->textures_len), free_screen(screen), free_preprocessing_data(data), free(data), free(screen), 1);
	}
	init_thread_pool(engine);
	init_static_lights(engine);

	engine->slr = malloc(sizeof(t_slr1));
	if (engine->slr)
	{
		initialize_data(engine->slr);
		engine->slr->table = create_parsing_table(engine->slr);
		if (!engine->slr->table)
			printf("Failed to generate SLR(1) parsing table\n");
		else
			init_all_ai_behaviors(engine->slr, engine);
	}

	// register hooks
	mlx_hook(screen->win_ptr, 2, 1L<<0, (int (*)())(void *)key_press, engine);
	mlx_hook(screen->win_ptr, 3, 1L<<1, (int (*)())(void *)key_release, engine);
	mlx_hook(screen->win_ptr, 17, 0L, (int (*)())(void *)close_window, engine);
	mlx_loop_hook(screen->mlx_ptr, (int (*)())(void *)game_loop, engine);

	// run game
	mlx_loop(screen->mlx_ptr);

	return (0);
}
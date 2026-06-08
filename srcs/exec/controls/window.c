#include "cub.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

static void	free_engine_graphics(t_engine *engine)
{
	if (engine->screen && engine->lamp_tex.img_ptr)
		mlx_destroy_image(engine->screen->mlx_ptr, engine->lamp_tex.img_ptr);
	if (engine->screen && engine->terminal_tex.img_ptr)
		mlx_destroy_image(engine->screen->mlx_ptr, engine->terminal_tex.img_ptr);
	if (engine->screen && engine->data)
		free_img_tab(engine->screen->mlx_ptr, engine->data->img_tab,
			engine->data->textures_len);
	if (engine->screen)
	{
		free_screen(engine->screen);
		free(engine->screen);
	}
	if (engine->data)
	{
		free_preprocessing_data(engine->data);
		free(engine->data);
	}
}

int	close_window(t_engine *engine)
{
	if (engine)
	{
		shutdown_thread_pool(engine);
		if (engine->slr)
			free_all(engine->slr, NULL);
		free(engine->player);
		free(engine->z_buffer);
		free(engine->blob);
		free_engine_graphics(engine);
	}
	exit(0);
	return (0);
}

static void	check_game_status(t_engine *engine)
{
	if (is_player_on_exit(engine))
	{
		printf("Victory! You reached the exit!\n");
		close_window(engine);
	}
	if (engine->player->hp <= 0)
	{
		printf("Game Over! You were defeated by a monster.\n");
		close_window(engine);
	}
}

int	game_loop(t_engine *engine)
{
	struct timeval	start;
	struct timeval	end;
	long			elapsed;

	gettimeofday(&start, NULL);
	update_doors(engine);
	update_interaction(engine);
	update_monsters(engine);
	if (engine->hacking_timer <= 0 && !engine->terminal_mode)
	{
		update_position(engine, &engine->keys);
		update_rotation(engine->player, &engine->keys);
	}
	check_game_status(engine);
	render_frame(engine);
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000000L
		+ (end.tv_usec - start.tv_usec);
	if (elapsed < 16666L)
		usleep(16666L - elapsed);
	return (0);
}

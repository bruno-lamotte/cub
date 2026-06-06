#include "cub.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

void	free_engine_graphics(t_engine *engine)
{
	int	i;

	i = -1;
	if (engine->screen && engine->data)
		free_img_tab(engine->screen->mlx_ptr, engine->data->img_tab,
			engine->data->textures_len);
	while (++i < engine->mstr_frame_count)
	 	mlx_destroy_image(engine->screen->mlx_ptr, engine->mstr_frames[i].img_ptr);
	free_screen(engine->screen);
	free_preprocessing_data(engine->data);
}

int	close_window(t_engine *engine, int ret_val)
{
	engine->ret_val = ret_val;
	mlx_loop_end(engine->screen->mlx_ptr);
	return (0);
}

bool is_player_on_exit(t_engine *engine)
{
	uint16_t x;
	uint16_t y;
	uint8_t	*flags;
	uint8_t	*occ;

	int idx;
	char sym;

	flags = get_map_flags(engine->blob);
	occ = get_map_occ_ids(engine->blob);

	x = (uint16_t)engine->player->pos.d.x;
	y = (uint16_t)engine->player->pos.d.y;

	idx = get_map_width((t_blob_hdr *)engine->blob) * y + x;
	if (!(flags[idx] & CELL_HAS_OBJ))
		return (false);

	sym = engine->data->obj_defs[occ[idx]].symbol;
	if (sym != 'X')
		return (false);

	return (true);
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
	if (is_player_on_exit(engine))
		close_window(engine, CUB_EXIT_MENU);
	if (engine->hacking_timer <= 0 && !engine->terminal_mode)
	{
		update_position(engine, &engine->keys);
		update_rotation(engine->player, &engine->keys);
	}
	render_frame(engine);
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000000L
		+ (end.tv_usec - start.tv_usec);
	if (elapsed < 16666L)
		usleep(16666L - elapsed);
	return (0);
}
